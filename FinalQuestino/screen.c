#include "game.h"

#define NEGATIVE_CLAMP_THETA 0.9999f

static void Screen_Reset( Screen_t* screen );
static void Screen_SetAddrWindow( Screen_t* screen, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );
static int8_t Screen_GetCharIndexFromChar( const char ch );
static uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y );

void Screen_Init( Screen_t* screen )
{
   uint8_t i;

   screen->csPort = portOutputRegister( digitalPinToPort( LCD_CS )  );
   screen->cdPort = portOutputRegister( digitalPinToPort( LCD_CD ) );
   screen->wrPort = portOutputRegister( digitalPinToPort( LCD_WR ) );
   screen->rdPort = portOutputRegister( digitalPinToPort( LCD_RD ) );
   screen->csPinSet = digitalPinToBitMask( LCD_CS );
   screen->cdPinSet = digitalPinToBitMask( LCD_CD );
   screen->wrPinSet = digitalPinToBitMask( LCD_WR );
   screen->rdPinSet = digitalPinToBitMask( LCD_RD );
   screen->csPinUnset = ~( screen->csPinSet );
   screen->cdPinUnset = ~( screen->cdPinSet );
   screen->wrPinUnset = ~( screen->wrPinSet );
   screen->rdPinUnset = ~( screen->rdPinSet );

   *( screen->csPort ) |= screen->csPinSet; // Set all control bits to HIGH (idle)
   *( screen->cdPort ) |= screen->cdPinSet; // Signals are ACTIVE LOW
   *( screen->wrPort ) |= screen->wrPinSet;
   *( screen->rdPort ) |= screen->rdPinSet;

   pinMode( LCD_CS, OUTPUT );    // Enable outputs
   pinMode( LCD_CD, OUTPUT );
   pinMode( LCD_WR, OUTPUT );
   pinMode( LCD_RD, OUTPUT );

   if ( LCD_RESET )
   {
      digitalWrite( LCD_RESET, HIGH );
      pinMode( LCD_RESET, OUTPUT );
   }

   setWriteDir(); // Set up LCD data port(s) for WRITE operations

   for ( i = 0; i < 16; i++ )
   {
      screen->mapPalette[i] = 0;
   }

   Screen_LoadTextBitFields( screen );

   screen->mapSpriteIndexCache = 0xFF;
}

static void Screen_Reset( Screen_t* screen )
{
   CS_IDLE;
   WR_IDLE;
   RD_IDLE;

   if( LCD_RESET )
   {
      digitalWrite( LCD_RESET, LOW );
      delay( 2 );
      digitalWrite( LCD_RESET, HIGH );
   }

   // Data transfer sync
   CS_ACTIVE;
   CD_COMMAND;
   write8( 0x00 );

   // Three extra 0x00s
   WR_STROBE;
   WR_STROBE;
   WR_STROBE;

   CS_IDLE;
}

void Screen_Begin( Screen_t* screen )
{
   // for WriteRegister16
   uint8_t hi, lo;

   Screen_Reset( screen );
   delay( 200 );

   CS_ACTIVE;
   writeRegister8( ILI9341_SOFTRESET, 0 );
   delay( 50 );
   writeRegister8( ILI9341_DISPLAYOFF, 0 );

   writeRegister8( ILI9341_POWERCONTROL1, 0x23 );
   writeRegister8( ILI9341_POWERCONTROL2, 0x10 );
   writeRegister16( ILI9341_VCOMCONTROL1, 0x2B2B );
   writeRegister8( ILI9341_VCOMCONTROL2, 0xC0 );
   writeRegister8( ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR );
   writeRegister8( ILI9341_PIXELFORMAT, 0x55 );
   writeRegister16( ILI9341_FRAMECONTROL, 0x001B );

   writeRegister8( ILI9341_ENTRYMODE, 0x07 );

   writeRegister8( ILI9341_SLEEPOUT, 0 );
   delay( 150 );
   writeRegister8( ILI9341_DISPLAYON, 0 );
   delay( 500 );
   Screen_SetAddrWindow( screen, 0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1 );
   CS_IDLE;

   // rotate the screen clockwise by 90 degrees
   CS_ACTIVE;
   screen->rotatedWidth = SCREEN_WIDTH;
   screen->rotatedHeight = SCREEN_HEIGHT;
   writeRegister8( ILI9341_MEMCONTROL, ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR );
   Screen_SetAddrWindow( screen, 0, 0, screen->rotatedWidth - 1, screen->rotatedHeight - 1 );
   CS_IDLE;
}

static void Screen_SetAddrWindow( Screen_t* screen, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
   CD_COMMAND;
   write8( ILI9341_COLADDRSET );
   CD_DATA;

   delayMicroseconds( 10 ); write8( x1 >> 8 );
   delayMicroseconds( 10 ); write8( x1 );
   delayMicroseconds( 10 ); write8( x2 >> 8 );
   delayMicroseconds( 10 ); write8( x2 );

   CD_COMMAND;
   write8( ILI9341_PAGEADDRSET );
   CD_DATA;

   delayMicroseconds( 10 ); write8( y1 >> 8 );
   delayMicroseconds( 10 ); write8( y1 );
   delayMicroseconds( 10 ); write8( y2 >> 8 );
   delayMicroseconds( 10 ); write8( y2 );
}

void Screen_DrawRect( Screen_t* screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color )
{
   uint8_t hi = color >> 8, lo = color;
   uint32_t pixelCount = (uint32_t)w * (uint32_t)h;
   uint32_t i;

   CS_ACTIVE;
   Screen_SetAddrWindow( screen, x, y, x + w - 1, y + h - 1 );
   CD_COMMAND;
   write8( 0x2C );
   CD_DATA;

   // write the first pixel (2 bytes)
   write16( hi, lo );

   if ( hi == lo )
   {
      // hi and lo bytes are the same, just strobe 2 bytes per pixel for the rest of the rect
      for ( i = 1; i < pixelCount; i++ )
      {
         WR_STROBE;
         WR_STROBE;
      }
   }
   else
   {
      // hi and lo bytes differ, need to write the whole color data out
      for ( i = 1; i < pixelCount; i++ )
      {
         write16( hi, lo );
      }
   }

   CS_IDLE;
}

void Screen_DrawTileMap( Game_t* game )
{
   uint16_t tileRow, tileCol, color, colorCache;
   uint8_t pixelRow, pixelCol, pixelPair, paletteIndex;
   uint8_t tile;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );

   CS_ACTIVE;
   Screen_SetAddrWindow( screen, 0, 0, ( MAP_TILE_SIZE * MAP_TILES_X ) - 1, ( MAP_TILE_SIZE * MAP_TILES_Y ) - 1 );
   CD_COMMAND;
   write8( 0x2C );
   CD_DATA;

   for ( tileRow = 0; tileRow < MAP_TILES_Y; tileRow++ )
   {
      for ( pixelRow = 0; pixelRow < MAP_TILE_SIZE; pixelRow++ )
      {
         for ( tileCol = 0; tileCol < MAP_TILES_X; tileCol++ )
         {
            tile = map->tiles[tileCol + ( tileRow * MAP_TILES_X )];

            for ( pixelCol = 0; pixelCol < MAP_PACKED_TILE_SIZE; pixelCol++ )
            {
               pixelPair = map->tileTextures[tile & 0x1F].pixels[pixelCol + ( pixelRow * MAP_PACKED_TILE_SIZE )];

               paletteIndex = pixelPair >> 4;
               color = screen->mapPalette[paletteIndex];
               write16( color >> 8, color );

               paletteIndex = pixelPair & 0x0F;
               color = screen->mapPalette[paletteIndex];
               write16( color >> 8, color );
            }
         }
      }
   }

   CS_IDLE;
}

static int8_t Screen_GetCharIndexFromChar( const char ch )
{
   if ( ch >= 97 && ch <= 122 )
   {
      // a - z (lower case letters start at 0 in our table)
      return ch - 97;
   }
   else if ( ch >= 65 && ch <= 90 )
   {
      // A - Z (upper case letters start at 26 in our table)
      return ch - 39;
   }
   else if ( ch >= 48 && ch <= 57 )
   {
      // 0 - 9 (numbers start at 52 in our table)
      return ch + 4;
   }
   else
   {
      // special characters start at 62 in our table
      switch ( ch )
      {
         case 44: return 62;     // comma
         case 33: return 63;     // exclamation point
         case 39: return 64;     // single quote
         case 38: return 65;     // ampersand
         case 46: return 66;     // period
         case 34: return 67;     // double quotes
         case 63: return 68;     // question mark
         case 45: return 69;     // dash
         case 62: return 70;     // greater-than
         case 58: return 71;     // colon
         case 47: return 72;     // forward slash

         default: return -1;
      }
   }
}

void Screen_DrawText( Screen_t* screen, const char* text, uint16_t x, uint16_t y, uint16_t backgroundColor, uint16_t foregroundColor )
{
   uint16_t ch;
   int8_t charIndex, i;
   uint8_t row;
   uint8_t* bitField;

   CS_ACTIVE;

   for ( ch = 0; ch < strlen( text ); ch++ )
   {
      Screen_SetAddrWindow( screen, x, y, x + 7, y + 7 );
      CD_COMMAND;
      write8( 0x2C );
      CD_DATA;

      charIndex = Screen_GetCharIndexFromChar( text[ch] );

      if ( charIndex < 0 )
      {
         for ( i = 0; i < 64; i++ )
         {
            write16( backgroundColor >> 8, backgroundColor );
         }
      }
      else
      {
         bitField = &( screen->textBitFields[charIndex] );

         for ( row = 0; row < 8; row++ )
         {
            for ( i = 7; i >= 0; i-- )
            {
               if ( bitField[row] & ( 0x01 << i ) )
               {
                  write16( foregroundColor >> 8, foregroundColor );
               }
               else
               {
                  write16( backgroundColor >> 8, backgroundColor );
               }
            }
         }
      }

      x += 8;
   }

   CS_IDLE;
}

void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint16_t x, uint16_t y,
                             uint8_t lineChars, uint8_t lineHeight,
                             uint16_t backgroundColor, uint16_t foregroundColor )
{
   uint8_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint16_t strLen = strlen( text );
   Bool_t endOfLine, endOfText;
   char line[32];
   char curChar;

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = text[textIndex];
      endOfLine = ( lineIndex == ( lineChars - 1 ) );
      endOfText = ( textIndex == ( strLen - 1 ) ) ? True : False;

      if ( endOfLine || endOfText )
      {
         if ( ( lastSpaceIndex > 0 ) && !endOfText )
         {
            if ( curChar == ' ' )
            {
               line[lineIndex] = '\0';
            }
            else if ( text[textIndex + 1] == ' ' )
            {
               line[lineIndex] = curChar;
               line[lineIndex + 1] = '\0';
            }
            else
            {
               line[lastSpaceIndex] = '\0';
               textIndex -= ( ( lineIndex - lastSpaceIndex ) + 1 );
            }
         }
         else if ( curChar == ' ' )
         {
            line[lineIndex] = '\0';
         }
         else
         {
            line[lineIndex] = curChar;
            line[lineIndex + 1] = '\0';
         }

         Screen_DrawText( screen, line, x, y + ( currentLine * lineHeight ), backgroundColor, foregroundColor );
         lineIndex = 0;
         lastSpaceIndex = 0;
         currentLine++;
      }
      else if ( curChar != ' ' )
      {
         line[lineIndex++] = curChar;
      }
      else if ( lineIndex > 0 && lastSpaceIndex != lineIndex )
      {
         line[lineIndex++] = curChar;
         lastSpaceIndex = lineIndex;
      }
   }
}

static uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y )
{
   uint8_t i, spriteIndex;
   uint16_t color;
   uint16_t tileIndex = ( ( y / MAP_TILE_SIZE ) * MAP_TILES_X ) + ( x / MAP_TILE_SIZE );
   TileMap_t* map = &( game->tileMap );
   uint8_t tile = map->tiles[tileIndex];
   uint8_t* tileTexture = &( map->tileTextures[tile & 0x1F].pixels );
   uint8_t pixelOffsetX = x % MAP_TILE_SIZE;
   uint8_t pixelOffsetY = y % MAP_TILE_SIZE;
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game, game->tileMapIndex, tileIndex );
   Screen_t* screen = &( game->screen );

   // check if this pixel is on a treasure that has already been collected
   if ( !( treasureFlag && !( game->treasureFlags & treasureFlag ) ) )
   {
      // if there's a sprite on this tile, check that first
      for ( i = 0; i < map->spriteCount; i++ )
      {
         if ( ( map->spriteData[i] & 0x1FF ) == tileIndex )
         {
            spriteIndex = ( map->spriteData[i] >> 9 ) & 0xF;

            if ( spriteIndex != screen->mapSpriteIndexCache )
            {
               TileMap_LoadSprite( map, spriteIndex );
               screen->mapSpriteIndexCache = spriteIndex;
            }

            color = pixelOffsetX % 2 == 0
               ? screen->mapPalette[map->spriteTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * SPRITE_PACKED_SIZE )] >> 4]
               : screen->mapPalette[map->spriteTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * SPRITE_PACKED_SIZE )] & 0xF];
            
            if ( color != TRANSPARENT_COLOR )
            {
               return color;
            }
         }
      }
   }

   if ( pixelOffsetX % 2 == 0 )
   {
      return screen->mapPalette[tileTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * MAP_PACKED_TILE_SIZE )] >> 4];
   }
   else
   {
      return screen->mapPalette[tileTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * MAP_PACKED_TILE_SIZE )] & 0xF];
   }
}

void Screen_DrawMapSprites( Game_t* game )
{
   uint8_t i, tileX, tileY, spriteIndex, pixelPair, paletteIndex;
   uint16_t tileIndex, color, j, pixel, x, y;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   uint32_t treasureFlag;

   CS_ACTIVE;

   for ( i = 0; i < map->spriteCount; i++ )
   {
      tileIndex = map->spriteData[i] & 0x1FF;
      treasureFlag = TileMap_GetTreasureFlag( game, game->tileMapIndex, tileIndex );

      if ( treasureFlag )
      {
         // this is a treasure chest that has already been collected
         if ( treasureFlag && !( game->treasureFlags & treasureFlag ) )
         {
            continue;
         }
      }

      spriteIndex = ( map->spriteData[i] >> 9 ) & 0xF;
      TileMap_LoadSprite( map, spriteIndex );
      game->screen.mapSpriteIndexCache = spriteIndex;

      tileY = ( tileIndex / MAP_TILES_X );
      tileX = ( tileIndex - ( tileY * MAP_TILES_X ) );
      x = tileX * MAP_TILE_SIZE;
      y = tileY * MAP_TILE_SIZE;

      Screen_SetAddrWindow( screen, x, y, x + SPRITE_SIZE - 1, y + SPRITE_SIZE - 1 );
      CD_COMMAND;
      write8( 0x2C );
      CD_DATA;

      for ( j = 0, pixel = 0; j < SPRITE_TEXTURE_SIZE_BYTES; j++ )
      {
         pixelPair = map->spriteTexture[j];
         paletteIndex = pixelPair >> 4;
         color = screen->mapPalette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }

         write16( color >> 8, color );
         pixel++;
         paletteIndex = pixelPair & 0x0F;
         color = screen->mapPalette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }

         write16( color >> 8, color );
         pixel++;
      }
   }

   CS_IDLE;
}

void Screen_DrawPlayer( Game_t* game )
{
   uint8_t pixelPair, paletteIndex, skipLeft, skipTop, skipRight, skipBottom, curX, curY;
   uint16_t startByte = ( (uint8_t)( game->player.sprite.direction ) * SPRITE_FRAMES * SPRITE_TEXTURE_SIZE_BYTES ) + ( game->player.sprite.currentFrame * SPRITE_TEXTURE_SIZE_BYTES );
   uint16_t color, i, pixel, ux, uy;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   float x = game->player.position.x + PLAYER_SPRITEOFFSET_X;
   float y = game->player.position.y + PLAYER_SPRITEOFFSET_Y;

   if ( x >= ( MAP_TILE_SIZE * MAP_TILES_X ) || y >= ( MAP_TILE_SIZE * MAP_TILES_Y ) || x + SPRITE_SIZE < 0 || y + SPRITE_SIZE < 0 )
   {
      return;
   }

   if ( x < 0 )
   {
      ux = 0;
      skipLeft = (uint8_t)( -( x - NEGATIVE_CLAMP_THETA ) );
      skipRight = 0;
   }
   else
   {
      ux = (uint16_t)x;
      skipLeft = 0;
      skipRight = ( ux + SPRITE_SIZE ) >= ( MAP_TILE_SIZE * MAP_TILES_X ) ? MAP_TILE_SIZE - ( ( MAP_TILE_SIZE * MAP_TILES_X ) - ux ) : 0;
   }

   if ( y < 0 )
   {
      uy = 0;
      skipTop = (uint8_t)( -( y - NEGATIVE_CLAMP_THETA ) );
      skipBottom = 0;
   }
   else
   {
      uy = (uint16_t)y;
      skipTop = 0;
      skipBottom = ( uy + SPRITE_SIZE ) >= ( MAP_TILE_SIZE * MAP_TILES_Y ) ? MAP_TILE_SIZE - ( ( MAP_TILE_SIZE * MAP_TILES_Y ) - uy ) : 0;
   }

   CS_ACTIVE;
   Screen_SetAddrWindow( screen, ux, uy, ux + SPRITE_SIZE - skipLeft - skipRight - 1, uy + SPRITE_SIZE - skipTop - skipBottom - 1 );
   CD_COMMAND;
   write8( 0x2C );
   CD_DATA;

   for ( i = startByte, pixel = 0, curX = 0, curY = 0; i < startByte + SPRITE_TEXTURE_SIZE_BYTES; i++ )
   {
      pixelPair = game->player.sprite.frameTextures[i];

      if ( curX >= skipLeft && curX < ( MAP_TILE_SIZE - skipRight ) && curY >= skipTop && curY < ( MAP_TILE_SIZE - skipBottom ) )
      {
         paletteIndex = pixelPair >> 4;
         color = screen->mapPalette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, ux + ( pixel % SPRITE_SIZE ), uy + ( pixel / SPRITE_SIZE ) );
         }

         write16( color >> 8, color );
      }
      
      pixel++;
      curX++;

      if ( curX >= skipLeft && curX < ( MAP_TILE_SIZE - skipRight ) && curY >= skipTop && curY < ( MAP_TILE_SIZE - skipBottom ) )
      {
         paletteIndex = pixelPair & 0x0F;
         color = screen->mapPalette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, ux + ( pixel % SPRITE_SIZE ), uy + ( pixel / SPRITE_SIZE ) );
         }

         write16( color >> 8, color );
      }
      
      pixel++;
      curX++;

      if ( curX >= SPRITE_SIZE )
      {
         curX = 0;
         curY++;

         if ( curY >= SPRITE_SIZE )
         {
            curY = 0;
         }
      }
   }

   CS_IDLE;
}

void Screen_WipePlayer( Game_t* game )
{
   Screen_WipeTileMapSection( game,
                              game->player.position.x + PLAYER_SPRITEOFFSET_X,
                              game->player.position.y + PLAYER_SPRITEOFFSET_Y,
                              SPRITE_SIZE, SPRITE_SIZE );
}

void Screen_DrawEnemy( Game_t* game, uint16_t x, uint16_t y )
{
   uint8_t i, j, pixelPair, paletteIndex;
   uint16_t tileOffsetX, tileOffsetY, tileX, tileY, color;
   Screen_t* screen = &( game->screen );
   Enemy_t* enemy = &( game->battle.enemy );

   CS_ACTIVE;

   for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
   {
      if ( enemy->tileTextureIndexes[i] >= 0 )
      {
         tileOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
         tileOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
         tileX = x + tileOffsetX;
         tileY = y + tileOffsetY;

         Screen_SetAddrWindow( screen, tileX, tileY, tileX + ENEMY_TILE_SIZE - 1, tileY + ENEMY_TILE_SIZE - 1 );
         CD_COMMAND;
         write8( 0x2C );
         CD_DATA;

         for ( j = 0; j < ENEMY_TILE_TEXTURE_SIZE_BYTES; j++ )
         {
            pixelPair = enemy->tileTextures[ enemy->tileTextureIndexes[i] ][j];

            paletteIndex = pixelPair >> 4;
            color = enemy->palette[paletteIndex];
            write16( color >> 8, color );

            paletteIndex = pixelPair & 0x0F;
            color = enemy->palette[paletteIndex];
            write16( color >> 8, color );
         }
      }
   }

   CS_IDLE;
}

void Screen_WipeTileMapSection( Game_t* game, float x, float y, uint16_t w, uint16_t h )
{
   uint8_t pixelPair, paletteIndex, curX, curY;
   uint16_t color;
   uint16_t ux, uy, row, col;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );

   if ( x >= ( MAP_TILE_SIZE * MAP_TILES_X ) || y >= ( MAP_TILE_SIZE * MAP_TILES_Y ) ||
        x + w < 0 || y + h < 0 )
   {
      return;
   }

   if ( x < 0 )
   {
      ux = 0;
      w = (uint16_t)( w + x );
   }
   else
   {
      ux = (uint16_t)x;
      w = ( ux + w ) >= ( MAP_TILE_SIZE * MAP_TILES_X ) ? ( MAP_TILE_SIZE * MAP_TILES_X ) - ux : w;
   }

   if ( y < 0 )
   {
      uy = 0;
      h = (uint16_t)( h + y );
   }
   else
   {
      uy = (uint16_t)y;
      h = ( uy + h ) >= ( MAP_TILE_SIZE * MAP_TILES_Y ) ? ( MAP_TILE_SIZE * MAP_TILES_Y ) - uy : h;
   }

   CS_ACTIVE;
   Screen_SetAddrWindow( screen, ux, uy, ux + w - 1, uy + h - 1 );
   CD_COMMAND;
   write8( 0x2C );
   CD_DATA;

   for ( row = uy; row < uy + h; row++ )
   {
      for ( col = ux; col < ux + w; col++ )
      {
         color = Screen_GetTilePixelColor( game, col, row );
         write16( color >> 8, color );
      }
   }

   CS_IDLE;
}
