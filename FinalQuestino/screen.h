#if !defined( SCREEN_H )
#define SCREEN_H

// NOTE: There's a LOT of useful information in Elegoo's touch-screen user manual,
// I stole all of this code from their library, you can probably find it by searching
// for "Elegoo 2.8 Inch Touch Screen User Manual".

#include "common.h"

// possibly useful color macros
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TRANSPARENT_COLOR MAGENTA

// registers
#define HX8347G_COLADDREND_HI   0x04
#define HX8347G_COLADDREND_LO   0x05
#define HX8347G_ROWADDREND_HI   0x08
#define HX8347G_ROWADDREND_LO   0x09
#define ILI9341_SOFTRESET       0x01
#define ILI9341_MADCTL_BGR      0x08
#define ILI9341_SLEEPOUT        0x11
#define ILI9341_DISPLAYOFF      0x28
#define ILI9341_DISPLAYON       0x29
#define ILI9341_COLADDRSET      0x2A
#define ILI9341_PAGEADDRSET     0x2B
#define ILI9341_MEMCONTROL      0x36
#define ILI9341_PIXELFORMAT     0x3A
#define ILI9341_MADCTL_MV       0x20
#define ILI9341_MADCTL_MX       0x40
#define ILI9341_MADCTL_MY       0x80
#define ILI9341_FRAMECONTROL    0xB1
#define ILI9341_ENTRYMODE       0xB7
#define ILI9341_POWERCONTROL1   0xC0
#define ILI9341_POWERCONTROL2   0xC1
#define ILI9341_VCOMCONTROL1    0xC5
#define ILI9341_VCOMCONTROL2    0xC7

// control pins
#define LCD_CS A3     // chip select
#define LCD_CD A2     // command/data
#define LCD_WR A1     // write
#define LCD_RD A0     // read
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

// Arduino Mega w/Breakout board
#define write8( d ) {\                        
	PORTH &= ~( 0x78 );\
 	PORTH |= ( ( d & 0xC0 ) >> 3 ) | ( ( d & 0x3 ) << 5 );\
 	PORTE &= ~( 0x38 );\
 	PORTE |= ( ( d & 0xC ) << 2 ) | ( ( d & 0x20 ) >> 2 );\
 	PORTG &= ~( 0x20 );\
 	PORTG |= ( d & 0x10 ) << 1; \
  WR_STROBE; }
#define write16( d1, d2 ) write8( d1 ) write8( d2 )
#define setWriteDir() { DDRH |= 0x78; DDRE |= 0x38; DDRG |= 0x20; }

// When using the TFT breakout board, control pins are configurable.
#define RD_ACTIVE  *( screen->rdPort ) &=  screen->rdPinUnset
#define RD_IDLE    *( screen->rdPort ) |=  screen->rdPinSet
#define WR_ACTIVE  *( screen->wrPort ) &=  screen->wrPinUnset
#define WR_IDLE    *( screen->wrPort ) |=  screen->wrPinSet
#define CD_COMMAND *( screen->cdPort ) &=  screen->cdPinUnset
#define CD_DATA    *( screen->cdPort ) |=  screen->cdPinSet
#define CS_ACTIVE  *( screen->csPort ) &=  screen->csPinUnset
#define CS_IDLE    *( screen->csPort ) |=  screen->csPinSet

// Data write strobe, ~2 instructions and always inline
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }

// Set value of TFT register: 8-bit address, 8-bit value
#define writeRegister8( a, d ) { CD_COMMAND; write8( a ); CD_DATA; write8( d ); }

// When using this, provide local variables uint8_t "hi" and "lo"
#define writeRegister16( a, d ) { \
  hi = ( a ) >> 8; lo = ( a ); CD_COMMAND; write8( hi ); write8( lo ); \
  hi = ( d ) >> 8; lo = ( d ); CD_DATA   ; write8( hi ); write8( lo ); }

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

typedef struct cGame_t cGame_t;

typedef struct cScreen_t
{
   uint16_t rotatedWidth;
   uint16_t rotatedHeight;

   volatile uint8_t* csPort;
   volatile uint8_t* cdPort;
   volatile uint8_t* wrPort;
   volatile uint8_t* rdPort;

   uint8_t csPinSet;
   uint8_t cdPinSet;
   uint8_t wrPinSet;
   uint8_t rdPinSet;

   uint8_t csPinUnset;
   uint8_t cdPinUnset;
   uint8_t wrPinUnset;
   uint8_t rdPinUnset;

   uint16_t mapPalette[16];
   uint8_t textBitFields[TEXT_TILE_COUNT][8];
}
cScreen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cScreen_Init( cScreen_t* screen );
void cScreen_Begin( cScreen_t* screen );
void cScreen_DrawRect( cScreen_t* screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color );
void cScreen_DrawTileMap( cGame_t* game );
void cScreen_DrawText( cScreen_t* screen, const char* text, uint16_t x, uint16_t y,
                       uint16_t backgroundColor, uint16_t foregroundColor );
void cScreen_DrawWrappedText( cScreen_t* screen, const char* text, uint16_t x, uint16_t y,
                              uint8_t lineChars, uint8_t lineHeight,
                              uint16_t backgroundColor, uint16_t foregroundColor );
void cScreen_DrawMapSprites( cGame_t* game );
void cScreen_DrawPlayer( cGame_t* game );
void cScreen_WipePlayer( cGame_t* game );
void cScreen_DrawEnemy( cGame_t* game, uint16_t x, uint16_t y );
void cScreen_WipeTileMapSection( cGame_t* game, float x, float y, uint16_t w, uint16_t h );

// data_loader.c
void cScreen_LoadMapPalette( cScreen_t* screen, uint8_t index );
void cScreen_LoadTextBitFields( cScreen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
