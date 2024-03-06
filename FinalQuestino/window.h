#if !defined( WINDOW_H )
#define WINDOW_H

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "sprite.h"

typedef struct cWindow_t
{
  cScreen_t screen;
  cTileMap_t tileMap;
  uint8_t textBitFields[TEXT_TILES][8];
  cSprite_t sprite;
}
cWindow_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cWindow_Init( cWindow_t* window );
void cWindow_Refresh( cWindow_t* window );

// data_loader.c
void cWindow_LoadTextBitFields( cWindow_t* window );

#if defined( __cplusplus )
}
#endif

#endif // WINDOW_H
