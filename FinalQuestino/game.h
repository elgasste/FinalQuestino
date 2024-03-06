#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "window.h"
#include "tile_map.h"
#include "clock.h"
#include "player.h"

typedef struct cGame_t
{
  cWindow_t window;
  cTileMap_t tileMap;
  uint8_t textBitFields[TEXT_TILES][8];

  cClock_t clock;
  cGameState_t state;
  cPlayer_t player;
}
cGame_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cGame_Init( cGame_t* game );
void cGame_Tic( cGame_t* game );

// data_loader.c
void cGame_LoadTextBitFields( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
