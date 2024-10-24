#include "screen.h"
#include "win_common.h"

internal uint32_t Convert565To32( uint16_t color )
{
   uint16_t r5 = ( ( color & 0xF800 ) >> 11 );
   uint16_t g6 = ( ( color & 0x07E0 ) >> 5 );
   uint16_t b5 = ( ( color & 0x001F ) );

   float pR = r5 == 0 ? 0.0f : 0x1F / r5;
   float pG = g6 == 0 ? 0.0f : 0x3F / g6;
   float pB = b5 == 0 ? 0.0f : 0x1F / b5;

   return (uint32_t)0xFF000000 | ( (uint32_t)( 0xFF * pR ) << 16 ) | ( (uint32_t)( 0xFF * pG ) << 8 ) | (uint32_t)( 0xFF * pB );
}

void Screen_Init( Screen_t* screen ) { }
void Screen_Begin( Screen_t* screen ) { }

void Screen_DrawRect( Screen_t* screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color )
{
   uint16_t i, j;
   WinPixelBuffer_t* buffer = &( g_globals.screenBuffer );
   uint32_t color32 = Convert565To32( color );
   uint32_t* memPos;

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

   memPos = buffer->memory + ( y * buffer->w ) + x;

   for ( j = 0; j < h; j++ )
   {
      for ( i = 0; i < w; i++ )
      {
         *memPos = color32;
         memPos++;
      }

      memPos += ( buffer->w - w );
   }
}

void Screen_DrawTileMap( Game_t* game )
{
   // TODO
}

void Screen_DrawText( Screen_t* screen, const char* text, uint16_t x, uint16_t y,
                      uint16_t backgroundColor, uint16_t foregroundColor )
{
   // TODO
}

void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint16_t x, uint16_t y,
                             uint8_t lineChars, uint8_t lineHeight,
                             uint16_t backgroundColor, uint16_t foregroundColor )
{
   // TODO
}

void Screen_DrawMapSprites( Game_t* game )
{
   // TODO
}

void Screen_DrawPlayer( Game_t* game )
{
   // TODO
}

void Screen_WipePlayer( Game_t* game )
{
   // TODO
}

void Screen_DrawEnemy( Game_t* game, uint16_t x, uint16_t y )
{
   // TODO
}

void Screen_WipeTileMapSection( Game_t* game, float x, float y, uint16_t w, uint16_t h )
{
   // TODO
}
