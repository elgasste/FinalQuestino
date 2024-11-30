#if !defined( SCREEN_H )
#define SCREEN_H

// NOTE: There's a LOT of useful information in Elegoo's touch-screen user manual,
// I stole all of this code from their library, you can probably find it by searching
// for "Elegoo 2.8 Inch Touch Screen User Manual".

#include "common.h"

// possibly useful color macros
#define BLACK     0x0000
#define DARKGRAY  0x4228
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF

#define TRANSPARENT_COLOR MAGENTA

#if !defined( VISUAL_STUDIO_DEV )

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E
#define HX8357_MADCTL  0x36

#define MADCTL_BGR 0x08
#define MADCTL_MV  0x20
#define MADCTL_MX  0x40

#define SETUP_CS_H PORTG |= _BV( 1 )
#define SETUP_CS_L PORTG &= ~_BV( 1 )

#define CS_H PORTG |= _BV( 1 )
#define CS_L PORTG &= ~_BV( 1 )

#define RS_H PORTD |= _BV( 7 )
#define RS_L PORTD &= ~_BV( 7 )

#define WR_H PORTG |= _BV( 2 );
#define WR_L PORTG &= ~_BV( 2 );
#define WR_STB PORTG &= ~_BV( 2 ); PORTG |= _BV( 2 );

#define WRITE_COLOR_16( c ) PORTC = c; PORTA = c >> 8; WR_STB;

#endif // VISUAL_STUDIO_DEV

#define NEGATIVE_CLAMP_THETA 0.9999f

#define LCD_WIDTH       480
#define LCD_HEIGHT      320
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define SCREEN_OFFSET_X 80
#define SCREEN_OFFSET_Y 40

typedef struct Game_t Game_t;

typedef struct Screen_t
{
   int8_t cs;
   int8_t rs;
   int8_t rst;
   int8_t wr;
   int8_t fcs;

   uint16_t palette[16];
   uint8_t textBitFields[TEXT_TILE_COUNT][8];

   uint8_t mapSpriteIndexCache;
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen );
void Screen_Clear();
void Screen_FillRect( uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color );
void Screen_DrawTileMap( Game_t* game );
void Screen_DrawTextLine( Screen_t* screen, const char* text, uint16_t x, uint16_t y, uint16_t backgroundColor, uint16_t foregroundColor );
void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint16_t x, uint16_t y, uint8_t lineChars, uint8_t lineHeight, uint16_t backgroundColor, uint16_t foregroundColor );
void Screen_DrawMapSprites( Game_t* game );
void Screen_DrawPlayer( Game_t* game );
void Screen_WipePlayer( Game_t* game );
void Screen_DrawEnemy( Game_t* game, uint16_t x, uint16_t y );
void Screen_WipeEnemy( Game_t* game, uint16_t x, uint16_t y );
void Screen_WipeTileMapSection( Game_t* game, float x, float y, uint16_t w, uint16_t h, Bool_t wipePlayer );
void Screen_WipeTileIndex( Game_t* game, uint16_t tileIndex, Bool_t wipePlayer );

// data_loader.c
void Screen_LoadMapPalette( Screen_t* screen, uint8_t index );
void Screen_LoadTextBitFields( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
