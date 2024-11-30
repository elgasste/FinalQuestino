#include "game.h"

internal void Screen_WriteCommand( uint8_t c );
internal void Screen_WriteData( uint8_t d );
internal void Screen_SetAddrWindow( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 );
internal void Screen_DrawPlayerSpriteSection( Game_t* game, uint8_t sx, uint8_t sy, uint8_t w, uint8_t h );
internal int8_t Screen_GetCharIndexFromChar( const char ch );
internal uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y );

void Screen_Init( Screen_t* screen )
{
   uint8_t i;

   screen->cs = 40; // PORT G bit _BV(1)
   screen->rs = 38; // PORT D bit _BV(7)
   screen->rst = 41; // PORT G bit _BV(0)
   screen->wr = 39; // PORT G bit _BV(2)
   screen->fcs  = 44; // FLASH chip select?

   pinMode( screen->rst, OUTPUT );
   digitalWrite( screen->rst, HIGH );

   pinMode( screen->rs, OUTPUT );
   pinMode( screen->cs, OUTPUT );
   pinMode( screen->wr, OUTPUT );

   digitalWrite( screen->rs, HIGH );
   digitalWrite( screen->cs, HIGH );
   digitalWrite( screen->wr, HIGH );

   pinMode( screen->fcs, OUTPUT );
   digitalWrite( screen->fcs, HIGH ); // stop line floating

   DDRA = 0xFF; // set direction for the 2 8 bit data ports
   DDRC = 0xFF;

   // toggle RST low to reset
   digitalWrite( screen->rst, HIGH );
   delay( 50 );
   digitalWrite( screen->rst, LOW );
   delay( 10 );
   digitalWrite( screen->rst, HIGH );
   delay( 10 );

   // configure HX8357-B display
   Screen_WriteCommand( 0x11 );
   delay( 20 );
   Screen_WriteCommand( 0xD0 );
   Screen_WriteData( 0x07 );
   Screen_WriteData( 0x42 );
   Screen_WriteData( 0x18 );

   Screen_WriteCommand( 0xD1 );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x07 );
   Screen_WriteData( 0x10 );

   Screen_WriteCommand( 0xD2 );
   Screen_WriteData( 0x01 );
   Screen_WriteData( 0x02 );

   Screen_WriteCommand( 0xC0 );
   Screen_WriteData( 0x10 );
   Screen_WriteData( 0x3B );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x02 );
   Screen_WriteData( 0x11 );

   Screen_WriteCommand( 0xC5 );
   Screen_WriteData( 0x08 );

   Screen_WriteCommand( 0xC8 );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x32 );
   Screen_WriteData( 0x36 );
   Screen_WriteData( 0x45 );
   Screen_WriteData( 0x06 );
   Screen_WriteData( 0x16 );
   Screen_WriteData( 0x37 );
   Screen_WriteData( 0x75 );
   Screen_WriteData( 0x77 );
   Screen_WriteData( 0x54 );
   Screen_WriteData( 0x0C );
   Screen_WriteData( 0x00 );

   Screen_WriteCommand( 0x36 );
   Screen_WriteData( 0x0a );

   Screen_WriteCommand( 0x3A );
   Screen_WriteData( 0x55 );

   Screen_WriteCommand( 0x2A );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x01 );
   Screen_WriteData( 0x3F );

   Screen_WriteCommand( 0x2B );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x00 );
   Screen_WriteData( 0x01 );
   Screen_WriteData( 0xDF );

   delay( 120 );
   Screen_WriteCommand( 0x29 );

   delay( 25 );

   // rotate
   Screen_WriteCommand( HX8357_MADCTL );
   Screen_WriteData( MADCTL_BGR | MADCTL_MV );

   for ( i = 0; i < 16; i++ )
   {
      screen->palette[i] = 0;
   }

   Screen_LoadTextBitFields( screen );

   screen->mapSpriteIndexCache = 0xFF;

   Screen_Clear();
}

internal void Screen_WriteCommand( uint8_t c )
{
   SETUP_CS_L;
   RS_L;
   PORTA = 0;
   PORTC = c;
   WR_STB;
   RS_H;
   SETUP_CS_H;
}

internal void Screen_WriteData( uint8_t d )
{
   SETUP_CS_L;
   PORTA = d >> 8;
   PORTC = d;
   WR_STB;
   SETUP_CS_H;
}

internal void Screen_SetAddrWindow( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 )
{
   x0 += SCREEN_OFFSET_X;
   x1 += SCREEN_OFFSET_X;

   y0 += SCREEN_OFFSET_Y;
   y1 += SCREEN_OFFSET_Y;

   CS_L;
   RS_L; PORTC = HX8357_CASET; WR_STB; RS_H;
   PORTC = x0 >> 8; WR_STB;
   PORTC = x0; WR_STB;
   PORTC = x1 >> 8; WR_STB;
   PORTC = x1; WR_STB;
   RS_L; PORTC = HX8357_PASET; WR_STB; RS_H;
   PORTC = y0 >> 8; WR_STB;
   PORTC = y0; WR_STB;
   PORTC = y1 >> 8; WR_STB;
   PORTC = y1; WR_STB;
   RS_L; PORTC = HX8357_RAMWR; WR_STB; RS_H;
}

void Screen_Clear()
{
   uint16_t x;
   uint16_t h = LCD_HEIGHT;

   CS_L;
   RS_L; PORTC = HX8357_CASET; WR_STB; RS_H;
   PORTC = 0 >> 8; WR_STB;
   PORTC = 0; WR_STB;
   PORTC = LCD_WIDTH >> 8; WR_STB;
   PORTC = LCD_WIDTH; WR_STB;
   RS_L; PORTC = HX8357_PASET; WR_STB; RS_H;
   PORTC = 0 >> 8; WR_STB;
   PORTC = 0; WR_STB;
   PORTC = LCD_HEIGHT >> 8; WR_STB;
   PORTC = LCD_HEIGHT; WR_STB;
   RS_L; PORTC = HX8357_RAMWR; WR_STB; RS_H;
   PORTC = BLACK;
   PORTA = BLACK >> 8;

   while( h-- )
   {
      x = LCD_WIDTH;

      while ( x > 15 )
      {
         x -= 16;
         WR_STB; WR_STB; WR_STB; WR_STB;
         WR_STB; WR_STB; WR_STB; WR_STB;
         WR_STB; WR_STB; WR_STB; WR_STB;
         WR_STB; WR_STB; WR_STB; WR_STB;
      }

      while( x-- )
      {
         WR_L; WR_H;
      }
   }

   CS_H;
}

void Screen_FillRect( uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color )
{
   Screen_SetAddrWindow( x, y, x + w - 1, y + h - 1 );
   PORTC = color;
   PORTA = color >> 8;

   if ( w > 15 )
   {
      while( h-- )
      {
         x = w;

         while ( x > 15 )
         {
            x -= 16;
            WR_STB; WR_STB; WR_STB; WR_STB;
            WR_STB; WR_STB; WR_STB; WR_STB;
            WR_STB; WR_STB; WR_STB; WR_STB;
            WR_STB; WR_STB; WR_STB; WR_STB;
         }

         while( x-- )
         {
            WR_L; WR_H;
         }
      }
   }
   else
   {
      while( h-- )
      {
         x = w;

         while( x-- )
         {
            WR_L; WR_H;
         }
      }
   }

   CS_H;
}

void Screen_DrawTileMap( Game_t* game )
{
   uint16_t tileRow, tileCol, color;
   uint8_t pixelRow, pixelCol, tileTextureIndex, pixelPair, paletteIndex;
   uint8_t tile;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );

   Screen_SetAddrWindow( 0, 0, ( MAP_TILE_SIZE * MAP_TILES_X ) - 1, ( MAP_TILE_SIZE * MAP_TILES_Y ) - 1 );

   for ( tileRow = 0; tileRow < MAP_TILES_Y; tileRow++ )
   {
      for ( pixelRow = 0; pixelRow < MAP_TILE_SIZE; pixelRow++ )
      {
         for ( tileCol = 0; tileCol < MAP_TILES_X; tileCol++ )
         {
            tile = map->tiles[tileCol + ( tileRow * MAP_TILES_X )];

            for ( pixelCol = 0; pixelCol < MAP_PACKED_TILE_SIZE; pixelCol++ )
            {
               tileTextureIndex = GET_TILE_TEXTURE_INDEX( tile );
               pixelPair = map->tileTextures[MIN_I( tileTextureIndex, 15 )].pixels[pixelCol + ( pixelRow * MAP_PACKED_TILE_SIZE )];

               paletteIndex = pixelPair >> 4;
               color = screen->palette[paletteIndex];
               WRITE_COLOR_16( color );

               paletteIndex = pixelPair & 0x0F;
               color = screen->palette[paletteIndex];
               WRITE_COLOR_16( color );
            }
         }
      }
   }

   CS_H;
}

internal int8_t Screen_GetCharIndexFromChar( const char ch )
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
         case 40: return 73;     // left parenthesis
         case 41: return 74;     // right parenthesis

         default: return -1;
      }
   }
}

void Screen_DrawTextLine( Screen_t* screen, const char* text, uint16_t x, uint16_t y, uint16_t backgroundColor, uint16_t foregroundColor )
{
   uint16_t ch, strLen;
   int8_t charIndex, i;
   uint8_t row, fieldValue;
   uint8_t *bitField;

   for ( ch = 0; ch < (uint16_t)strlen( text ); ch++ )
   {
      Screen_SetAddrWindow( x, y, x + 7, y + 7 );
      charIndex = Screen_GetCharIndexFromChar( text[ch] );

      if ( charIndex < 0 )
      {
         for ( i = 0; i < 64; i++ )
         {
            WRITE_COLOR_16( backgroundColor );
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
                  WRITE_COLOR_16( foregroundColor );
               }
               else
               {
                  WRITE_COLOR_16( backgroundColor );
               }
            }
         }
      }

      CS_H;
      x += 8;
   }
}

void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint16_t x, uint16_t y, uint8_t lineChars, uint8_t lineHeight, uint16_t backgroundColor, uint16_t foregroundColor )
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

         Screen_DrawTextLine( screen, line, x, y + ( currentLine * lineHeight ), backgroundColor, foregroundColor );
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

// TODO: look into ways to optimize this
internal uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y )
{
   uint8_t i, tileTextureIndex, spriteIndex;
   uint16_t color;
   uint16_t tileIndex = ( ( y / MAP_TILE_SIZE ) * MAP_TILES_X ) + ( x / MAP_TILE_SIZE );
   TileMap_t* map = &( game->tileMap );
   uint8_t tile = map->tiles[tileIndex];
   uint8_t *tileTexture;
   uint8_t pixelOffsetX = x % MAP_TILE_SIZE;
   uint8_t pixelOffsetY = y % MAP_TILE_SIZE;
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game->tileMapIndex, tileIndex );
   uint32_t doorFlag = TileMap_GetDoorFlag( game->tileMapIndex, tileIndex );
   Screen_t* screen = &( game->screen );

   tileTextureIndex = GET_TILE_TEXTURE_INDEX( tile );
   tileTexture = &( map->tileTextures[MIN_I( tileTextureIndex, 15 )].pixels );

   // check if this pixel is on a treasure that has already been collected, or a door that has already been opened
   if ( !( treasureFlag && !( game->treasureFlags & treasureFlag ) ) && !( doorFlag && !( game->doorFlags & doorFlag ) ) )
   {
      // check if there's a sprite on this tile
      for ( i = 0; i < map->spriteCount; i++ )
      {
         if ( ( GET_SPRITE_TILE_INDEX( map->spriteData[i] ) ) == tileIndex )
         {
            spriteIndex = GET_SPRITE_INDEX( map->spriteData[i] );

            if ( spriteIndex != screen->mapSpriteIndexCache )
            {
               TileMap_LoadSprite( map, spriteIndex );
               screen->mapSpriteIndexCache = spriteIndex;
            }

            color = pixelOffsetX % 2 == 0
               ? screen->palette[map->spriteTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * SPRITE_PACKED_SIZE )] >> 4]
               : screen->palette[map->spriteTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * SPRITE_PACKED_SIZE )] & 0xF];
            
            if ( color != TRANSPARENT_COLOR )
            {
               return color;
            }
         }
      }
   }

   if ( pixelOffsetX % 2 == 0 )
   {
      return screen->palette[tileTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * MAP_PACKED_TILE_SIZE )] >> 4];
   }
   else
   {
      return screen->palette[tileTexture[( pixelOffsetX / 2 ) + ( pixelOffsetY * MAP_PACKED_TILE_SIZE )] & 0xF];
   }
}

void Screen_DrawMapSprites( Game_t* game )
{
   uint8_t i, tileX, tileY, spriteIndex, pixelPair, paletteIndex;
   uint16_t tileIndex, color, j, pixel, x, y;
   TileMap_t* map = &( game->tileMap );
   uint32_t treasureFlag, doorFlag;
   Screen_t* screen = &( game->screen );

   for ( i = 0; i < map->spriteCount; i++ )
   {
      tileIndex = GET_SPRITE_TILE_INDEX( map->spriteData[i] );
      treasureFlag = TileMap_GetTreasureFlag( game->tileMapIndex, tileIndex );
      doorFlag = TileMap_GetDoorFlag( game->tileMapIndex, tileIndex );

      if ( treasureFlag )
      {
         // this is a treasure chest that has already been collected
         if ( treasureFlag && !( game->treasureFlags & treasureFlag ) )
         {
            continue;
         }
      }
      else if ( doorFlag )
      {
         // this is a door that has already been opened
         if ( doorFlag && !( game->doorFlags & doorFlag ) )
         {
            continue;
         }
      }

      spriteIndex = GET_SPRITE_INDEX( map->spriteData[i] );
      TileMap_LoadSprite( map, spriteIndex );
      game->screen.mapSpriteIndexCache = spriteIndex;

      tileY = (uint8_t)( ( tileIndex / MAP_TILES_X ) );
      tileX = (uint8_t)( ( tileIndex - ( tileY * MAP_TILES_X ) ) );
      x = tileX * MAP_TILE_SIZE;
      y = tileY * MAP_TILE_SIZE;

      Screen_SetAddrWindow( x, y, x + SPRITE_SIZE - 1, y + SPRITE_SIZE - 1 );

      for ( j = 0, pixel = 0; j < SPRITE_TEXTURE_SIZE_BYTES; j++ )
      {
         pixelPair = map->spriteTexture[j];
         paletteIndex = pixelPair >> 4;
         color = screen->palette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }

         WRITE_COLOR_16( color );
         pixel++;
         paletteIndex = pixelPair & 0x0F;
         color = screen->palette[paletteIndex];

         if ( color == TRANSPARENT_COLOR )
         {
            color = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }

         WRITE_COLOR_16( color );
         pixel++;
      }

      CS_H;
   }
}

void Screen_DrawPlayer( Game_t* game )
{
   Screen_DrawPlayerSpriteSection( game, 0, 0, SPRITE_SIZE, SPRITE_SIZE );
}

void Screen_WipePlayer( Game_t* game )
{
   Screen_WipeTileMapSection( game,
                              game->player.position.x + PLAYER_SPRITEOFFSET_X,
                              game->player.position.y + PLAYER_SPRITEOFFSET_Y,
                              SPRITE_SIZE, SPRITE_SIZE,
                              True );
}

void Screen_DrawEnemy( Game_t* game, uint16_t x, uint16_t y )
{
   uint8_t i, j, pixelPair, paletteIndex;
   uint16_t tileOffsetX, tileOffsetY, tileX, tileY, color;
   Screen_t* screen = &( game->screen );
   Enemy_t* enemy = &( game->battle.enemy );

   // MUFFINS: call SetAddrWindow once, with the total enemy size (figure out what that is)

   // for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
   // {
   //    if ( enemy->tileTextureIndexes[i] >= 0 )
   //    {
   //       tileOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
   //       tileOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
   //       tileX = x + tileOffsetX;
   //       tileY = y + tileOffsetY;

   //       Screen_SetAddrWindow( screen, tileX, tileY, tileX + ENEMY_TILE_SIZE - 1, tileY + ENEMY_TILE_SIZE - 1 );
   //       CD_COMMAND;
   //       write8( 0x2C );
   //       CD_DATA;

   //       for ( j = 0; j < ENEMY_TILE_TEXTURE_SIZE_BYTES; j++ )
   //       {
   //          pixelPair = enemy->tileTextures[ enemy->tileTextureIndexes[i] ][j];

   //          paletteIndex = pixelPair >> 4;
   //          color = enemy->palette[paletteIndex];
   //          write16( color >> 8, color );

   //          paletteIndex = pixelPair & 0x0F;
   //          color = enemy->palette[paletteIndex];
   //          write16( color >> 8, color );
   //       }
   //    }
   // }

   // CS_IDLE;
}

void Screen_WipeEnemy( Game_t* game, uint16_t x, uint16_t y )
{
   // uint8_t i, j;
   // uint16_t tileOffsetX, tileOffsetY, tileX, tileY;
   // Screen_t* screen = &( game->screen );
   // Enemy_t* enemy = &( game->battle.enemy );

   // CS_ACTIVE;

   // for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
   // {
   //    if ( enemy->tileTextureIndexes[i] >= 0 )
   //    {
   //       tileOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
   //       tileOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
   //       tileX = x + tileOffsetX;
   //       tileY = y + tileOffsetY;

   //       Screen_SetAddrWindow( screen, tileX, tileY, tileX + ENEMY_TILE_SIZE - 1, tileY + ENEMY_TILE_SIZE - 1 );
   //       CD_COMMAND;
   //       write8( 0x2C );
   //       CD_DATA;

   //       for ( j = 0; j < ENEMY_TILE_TEXTURE_SIZE_BYTES; j++ )
   //       {
   //          write16( BLACK >> 8, BLACK );
   //          write16( BLACK >> 8, BLACK );
   //       }
   //    }
   // }

   // CS_IDLE;
}

void Screen_WipeTileMapSection( Game_t* game, float x, float y, uint16_t w, uint16_t h, Bool_t wipePlayer )
{
   int16_t sx, sy, xOffset, yOffset, redrawX, redrawY, redrawW, redrawH;
   uint16_t color, ux, uy, row, col;

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

   Screen_SetAddrWindow( ux, uy, ux + w - 1, uy + h - 1 );

   for ( row = uy; row < uy + h; row++ )
   {
      for ( col = ux; col < ux + w; col++ )
      {
         color = Screen_GetTilePixelColor( game, col, row );
         WRITE_COLOR_16( color );
      }
   }

   CS_H;

   if ( !wipePlayer )
   {
      sx = (int16_t)( game->player.position.x ) + PLAYER_SPRITEOFFSET_X;
      sy = (int16_t)( game->player.position.y ) + PLAYER_SPRITEOFFSET_Y;

      if ( sx <= ( x + w ) && ( sx + SPRITE_SIZE ) >= x && sy <= ( y + h ) && ( sy + SPRITE_SIZE ) > y )
      {
         xOffset = (int16_t)( sx - x );
         yOffset = (int16_t)( sy - y );
         redrawX = 0;
         redrawY = 0;
         redrawW = SPRITE_SIZE;
         redrawH = SPRITE_SIZE;

         if ( xOffset < 0 )
         {
            redrawX = -xOffset;
            redrawW = SPRITE_SIZE - redrawX;
         }
         else if ( ( xOffset + SPRITE_SIZE ) > ( x + w ) )
         {
            redrawW = SPRITE_SIZE - ( xOffset % SPRITE_SIZE );
         }

         if ( yOffset < 0 )
         {
            redrawY = -yOffset;
            redrawH = SPRITE_SIZE - redrawY;
         }
         else if ( ( yOffset + SPRITE_SIZE ) > ( y + h ) )
         {
            redrawH = SPRITE_SIZE - ( yOffset % SPRITE_SIZE );
         }

         Screen_DrawPlayerSpriteSection( game, (uint8_t)redrawX, (uint8_t)redrawY, (uint8_t)redrawW, (uint8_t)redrawH );
      }
   }
}

void Screen_WipeTileIndex( Game_t* game, uint16_t tileIndex, Bool_t wipePlayer )
{
   uint8_t y = ( uint8_t )( tileIndex / MAP_TILES_X );
   uint8_t x = ( uint8_t )( tileIndex - ( y * MAP_TILES_X ) );

   Screen_WipeTileMapSection( game, (float)x * MAP_TILE_SIZE, (float)y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE, wipePlayer );
}

internal void Screen_DrawPlayerSpriteSection( Game_t* game, uint8_t sx, uint8_t sy, uint8_t w, uint8_t h )
{
   uint8_t pixelPair, paletteIndex, skipLeft, skipTop, skipRight, skipBottom, curX, curY;
   uint16_t b, startByte, color, pixel, ux, uy, tx, ty;
   float px = game->player.position.x + PLAYER_SPRITEOFFSET_X;
   float py = game->player.position.y + PLAYER_SPRITEOFFSET_Y;
   Screen_t* screen = &( game->screen );

   if ( px >= ( MAP_TILE_SIZE * MAP_TILES_X ) || py >= ( MAP_TILE_SIZE * MAP_TILES_Y ) || px + SPRITE_SIZE < 0 || py + SPRITE_SIZE < 0 )
   {
      return;
   }

   if ( px >= ( MAP_TILE_SIZE * MAP_TILES_X ) || py >= ( MAP_TILE_SIZE * MAP_TILES_Y ) || px + SPRITE_SIZE < 0 || py + SPRITE_SIZE < 0 )
   {
      return;
   }

   if ( px < 0 )
   {
      ux = 0;
      skipLeft = (uint8_t)( -( px - NEGATIVE_CLAMP_THETA ) );
      if ( skipLeft > sx )
      {
         sx = skipLeft;
         w -= skipLeft;
      }
      skipRight = SPRITE_SIZE - ( sx + w );
   }
   else
   {
      ux = (uint16_t)px + sx;
      skipLeft = sx;
      skipRight = ( ux + SPRITE_SIZE ) >= ( MAP_TILE_SIZE * MAP_TILES_X ) ? (uint8_t)( MAP_TILE_SIZE - ( ( MAP_TILE_SIZE * MAP_TILES_X ) - ux ) ) : 0;
      w -= skipRight;
   }

   if ( py < 0 )
   {
      uy = 0;
      skipTop = (uint8_t)( -( py - NEGATIVE_CLAMP_THETA ) );
      if ( skipTop > sy )
      {
         sy = skipTop;
         h -= skipTop;
      }
      skipBottom = SPRITE_SIZE - ( sy + h );
   }
   else
   {
      uy = (uint16_t)py + sy;
      skipTop = sy;
      skipBottom = ( uy + SPRITE_SIZE ) >= ( MAP_TILE_SIZE * MAP_TILES_Y ) ? (uint8_t)( MAP_TILE_SIZE - ( ( MAP_TILE_SIZE * MAP_TILES_Y ) - uy ) ) : 0;
      h -= skipBottom;
   }

   startByte = ( (uint8_t)( game->player.sprite.direction ) * SPRITE_FRAMES * SPRITE_TEXTURE_SIZE_BYTES )
      + ( game->player.sprite.currentFrame * SPRITE_TEXTURE_SIZE_BYTES )
      + ( skipLeft / 2 )
      + ( skipTop * 8 );

   Screen_SetAddrWindow( ux, uy, ux + w - 1, uy + h - 1 );

   for ( b = startByte, pixel = 0; b < ( startByte + SPRITE_TEXTURE_SIZE_BYTES ) && sy < ( SPRITE_SIZE - skipBottom ); b++ )
   {
      pixelPair = game->player.sprite.frameTextures[b];
      paletteIndex = pixelPair >> 4;
      color = screen->palette[paletteIndex];

      if ( color == TRANSPARENT_COLOR )
      {
         tx = ux + ( pixel % SPRITE_SIZE );
         ty = uy + ( pixel / SPRITE_SIZE );
         color = Screen_GetTilePixelColor( game, tx, ty );
      }

      WRITE_COLOR_16( color );
      sx++;
      pixel++;

      if ( sx >= SPRITE_SIZE - skipRight )
      {
         sx = skipLeft;
         sy++;
         pixel += skipRight + skipLeft;
         b += ( skipRight / 2 ) + ( skipLeft / 2 );
         continue;
      }

      paletteIndex = pixelPair & 0x0F;
      color = screen->palette[paletteIndex];

      if ( color == TRANSPARENT_COLOR )
      {
         tx = ux + ( pixel % SPRITE_SIZE );
         ty = uy + ( pixel / SPRITE_SIZE );
         color = Screen_GetTilePixelColor( game, tx, ty );
      }

      WRITE_COLOR_16( color );
      sx++;
      pixel++;

      if ( sx >= SPRITE_SIZE - skipRight )
      {
         sx = skipLeft;
         sy++;
         pixel += skipRight + skipLeft;
         b += ( skipRight / 2 ) + ( skipLeft / 2 );
      }
   }

   CS_H;
}
