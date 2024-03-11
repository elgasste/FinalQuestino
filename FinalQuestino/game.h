#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "clock.h"
#include "input.h"
#include "physics.h"
#include "player.h"

typedef struct cGame_t
{
  cScreen_t screen;
  cTileMap_t tileMap;
  uint8_t paletteIndex;
  uint8_t tileTexturesIndex;
  uint8_t tileMapIndex;

  cClock_t clock;
  cInput_t input;
  cPhysics_t physics;

  cGameState_t state;
  cPlayer_t player;
}
cGame_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cGame_Init( cGame_t* game );
void cGame_Tic( cGame_t* game );
void cGame_SteppedOnTile( cGame_t* game, uint16_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
