#include "screen.h"
#include "win_common.h"

internal uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y );
internal int8_t Screen_GetCharIndexFromChar( const char c );

internal uint32_t Convert565To32( uint16_t color )
{
   if ( color == 0 )
   {
      return 0;
   }

   uint16_t r5 = ( ( color & 0xF800 ) >> 11 );
   uint16_t g6 = ( ( color & 0x07E0 ) >> 5 );
   uint16_t b5 = ( ( color & 0x001F ) );

   float pR = ( r5 == 0 ) ? 0.0f : (float)r5 / 0x1F;
   float pG = ( g6 == 0 ) ? 0.0f : (float)g6 / 0x3F;
   float pB = ( b5 == 0 ) ? 0.0f : (float)b5 / 0x1F;

   return (uint32_t)0xFF000000 | ( (uint32_t)( 0xFF * pR ) << 16 ) | ( (uint32_t)( 0xFF * pG ) << 8 ) | (uint32_t)( 0xFF * pB );
}

void Screen_Init( Screen_t* screen )
{
   uint8_t i;

   for ( i = 0; i < 16; i++ )
   {
      screen->mapPalette[i] = 0;
   }

   Screen_LoadTextBitFields( screen );
   screen->mapSpriteIndexCache = 0xFF;
}

void Screen_Begin( Screen_t* screen )
{
   // this is for starting up the LCD screen, not necessary here
}

void Screen_DrawRect( Screen_t* screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color )
{
   uint16_t i, j;
   WinPixelBuffer_t* buffer = &( g_globals.screenBuffer );
   uint32_t color32 = Convert565To32( color );
   uint32_t* bufferPos;

   if ( x >= buffer->w || y >= buffer->h )
   {
      return;
   }

   if ( (uint32_t)( x + w ) >= buffer->w )
   {
      w = buffer->w - x;
   }
   if ( (uint32_t)( y + h ) >= buffer->h )
   {
      h = buffer->h - y;
   }

   bufferPos = buffer->memory + ( y * buffer->w ) + x;

   for ( j = 0; j < h; j++ )
   {
      for ( i = 0; i < w; i++ )
      {
         *bufferPos = color32;
         bufferPos++;
      }

      bufferPos += ( buffer->w - w );
   }
}

void Screen_DrawTileMap( Game_t* game )
{
   uint16_t tileRow, tileCol;
   uint32_t color32;
   uint8_t pixelRow, pixelCol, pixelPair, paletteIndex;
   uint8_t tile;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   uint32_t* bufferPos = g_globals.screenBuffer.memory;

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
               color32 = Convert565To32( screen->mapPalette[paletteIndex] );
               *bufferPos = color32;
               bufferPos++;

               paletteIndex = pixelPair & 0x0F;
               color32 = Convert565To32( screen->mapPalette[paletteIndex] );
               *bufferPos = color32;
               bufferPos++;
            }
         }
      }
   }
}

void Screen_DrawText( Screen_t* screen, const char* text, uint16_t x, uint16_t y,
                      uint16_t backgroundColor, uint16_t foregroundColor )
{
   uint16_t ch, j;
   int8_t charIndex, i;
   uint8_t row;
   uint32_t color32;
   uint8_t* bitField;
   uint32_t* bufferPos;

   for ( ch = 0; ch < strlen( text ); ch++ )
   {
      bufferPos = g_globals.screenBuffer.memory + ( y * g_globals.screenBuffer.w ) + x;
      charIndex = Screen_GetCharIndexFromChar( text[ch] );

      if ( charIndex < 0 )
      {
         for ( i = 0, j = 0; i < 64; i++ )
         {
            color32 = Convert565To32( backgroundColor );
            *bufferPos = color32;
            bufferPos++;
            j++;

            if ( j == 8 )
            {
               bufferPos += ( g_globals.screenBuffer.w - 8 );
               j = 0;
            }
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
                  color32 = Convert565To32( foregroundColor );
                  *bufferPos = color32;
               }
               else
               {
                  color32 = Convert565To32( backgroundColor );
                  *bufferPos = color32;
               }

               bufferPos++;
            }

            bufferPos += ( g_globals.screenBuffer.w - 8 );
         }
      }

      x += 8;
   }
}

void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint16_t x, uint16_t y,
                             uint8_t lineChars, uint8_t lineHeight,
                             uint16_t backgroundColor, uint16_t foregroundColor )
{
   uint8_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint16_t strLen = (uint16_t)strlen( text );
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

void Screen_DrawMapSprites( Game_t* game )
{
   uint8_t i, tileX, tileY, spriteIndex, pixelPair, paletteIndex;
   uint16_t tileIndex, color16, j, pixel, x, y;
   uint32_t color32;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   uint32_t treasureFlag;
   uint32_t* bufferPos;

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
      bufferPos = g_globals.screenBuffer.memory + ( y * g_globals.screenBuffer.w ) + x;

      for ( j = 0, pixel = 0; j < SPRITE_TEXTURE_SIZE_BYTES; j++ )
      {
         pixelPair = map->spriteTexture[j];
         paletteIndex = pixelPair >> 4;
         color16 = screen->mapPalette[paletteIndex];
         if ( color16 == TRANSPARENT_COLOR )
         {
            color16 = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }
         color32 = Convert565To32( color16 );

         *bufferPos = color32;
         pixel++;
         bufferPos++;

         paletteIndex = pixelPair & 0x0F;
         color16 = screen->mapPalette[paletteIndex];
         if ( color16 == TRANSPARENT_COLOR )
         {
            color16 = Screen_GetTilePixelColor( game, x + ( pixel % SPRITE_SIZE ), y + ( pixel / SPRITE_SIZE ) );
         }
         color32 = Convert565To32( color16 );

         *bufferPos = color32;
         pixel++;
         bufferPos++;

         if ( pixel % SPRITE_SIZE == 0 )
         {
            bufferPos += ( g_globals.screenBuffer.w - SPRITE_SIZE );
         }
      }
   }
}

void Screen_DrawPlayer( Game_t* game )
{
   uint8_t pixelPair, paletteIndex, skipLeft, skipTop, skipRight, skipBottom, curX, curY;
   uint16_t startByte = ( (uint8_t)( game->player.sprite.direction ) * SPRITE_FRAMES * SPRITE_TEXTURE_SIZE_BYTES ) + ( game->player.sprite.currentFrame * SPRITE_TEXTURE_SIZE_BYTES );
   uint16_t color16, i, pixel, ux, uy;
   uint32_t color32;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   float x = game->player.position.x + PLAYER_SPRITEOFFSET_X;
   float y = game->player.position.y + PLAYER_SPRITEOFFSET_Y;
   uint32_t* bufferPos;

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

   bufferPos = g_globals.screenBuffer.memory + ( uy * g_globals.screenBuffer.w ) + ux;

   for ( i = startByte, pixel = 0, curX = 0, curY = 0; i < startByte + SPRITE_TEXTURE_SIZE_BYTES; i++ )
   {
      pixelPair = game->player.sprite.frameTextures[i];

      if ( curX >= skipLeft && curX < ( MAP_TILE_SIZE - skipRight ) && curY >= skipTop && curY < ( MAP_TILE_SIZE - skipBottom ) )
      {
         paletteIndex = pixelPair >> 4;
         color16 = screen->mapPalette[paletteIndex];
         if ( color16 == TRANSPARENT_COLOR )
         {
            color16 = Screen_GetTilePixelColor( game, ux + ( pixel % SPRITE_SIZE ), uy + ( pixel / SPRITE_SIZE ) );
         }
         color32 = Convert565To32( color16 );
         *bufferPos = color32;
      }

      pixel++;
      curX++;
      bufferPos++;

      if ( curX >= skipLeft && curX < ( MAP_TILE_SIZE - skipRight ) && curY >= skipTop && curY < ( MAP_TILE_SIZE - skipBottom ) )
      {
         paletteIndex = pixelPair & 0x0F;
         color16 = screen->mapPalette[paletteIndex];
         if ( color16 == TRANSPARENT_COLOR )
         {
            color16 = Screen_GetTilePixelColor( game, ux + ( pixel % SPRITE_SIZE ), uy + ( pixel / SPRITE_SIZE ) );
         }
         color32 = Convert565To32( color16 );
         *bufferPos = color32;
      }

      pixel++;
      curX++;
      bufferPos++;

      if ( curX >= SPRITE_SIZE )
      {
         curX = 0;
         curY++;
         bufferPos += ( g_globals.screenBuffer.w - SPRITE_SIZE );

         if ( curY >= SPRITE_SIZE )
         {
            curY = 0;
         }
      }
   }
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
   uint8_t i, j, k, pixelPair, paletteIndex;
   uint16_t tileOffsetX, tileOffsetY, tileX, tileY, color16;
   uint32_t color32;
   Enemy_t* enemy = &( game->battle.enemy );
   uint32_t* bufferPos;

   for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
   {
      if ( enemy->tileTextureIndexes[i] >= 0 )
      {
         tileOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
         tileOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
         tileX = x + tileOffsetX;
         tileY = y + tileOffsetY;

         bufferPos = g_globals.screenBuffer.memory + ( tileY * g_globals.screenBuffer.w ) + tileX;

         for ( j = 0, k = 0; j < ENEMY_TILE_TEXTURE_SIZE_BYTES; j++ )
         {
            pixelPair = enemy->tileTextures[ enemy->tileTextureIndexes[i] ][j];

            paletteIndex = pixelPair >> 4;
            color16 = enemy->palette[paletteIndex];
            color32 = Convert565To32( color16 );
            *bufferPos = color32;
            bufferPos++;

            paletteIndex = pixelPair & 0x0F;
            color16 = enemy->palette[paletteIndex];
            color32 = Convert565To32( color16 );
            *bufferPos = color32;
            bufferPos++;

            k += 2;

            if ( k == ENEMY_TILE_SIZE )
            {
               bufferPos += ( g_globals.screenBuffer.w - ENEMY_TILE_SIZE );
               k = 0;
            }
         }
      }
   }
}

void Screen_WipeTileMapSection( Game_t* game, float x, float y, uint16_t w, uint16_t h )
{
   uint16_t color16, ux, uy, row, col;
   uint32_t color32;
   Screen_t* screen = &( game->screen );
   TileMap_t* map = &( game->tileMap );
   uint32_t* bufferPos;

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

   bufferPos = g_globals.screenBuffer.memory + ( uy * g_globals.screenBuffer.w ) + ux;

   for ( row = uy; row < uy + h; row++ )
   {
      for ( col = ux; col < ux + w; col++ )
      {
         color16 = Screen_GetTilePixelColor( game, col, row );
         color32 = Convert565To32( color16 );
         *bufferPos = color32;
         bufferPos++;
      }

      bufferPos += ( g_globals.screenBuffer.w - w );
   }
}

internal uint16_t Screen_GetTilePixelColor( Game_t* game, uint16_t x, uint16_t y )
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

internal int8_t Screen_GetCharIndexFromChar( const char c )
{
   if ( c >= 97 && c <= 122 )
   {
      // a - z (lower case letters start at 0 in our table)
      return c - 97;
   }
   else if ( c >= 65 && c <= 90 )
   {
      // A - Z (upper case letters start at 26 in our table)
      return c - 39;
   }
   else if ( c >= 48 && c <= 57 )
   {
      // 0 - 9 (numbers start at 52 in our table)
      return c + 4;
   }
   else
   {
      // special characters start at 62 in our table
      switch ( c )
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
