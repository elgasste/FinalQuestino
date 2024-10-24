#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "menu.h"
#include "clock.h"
#include "input.h"
#include "physics.h"
#include "player.h"
#include "battle.h"

typedef struct Game_t
{
   Screen_t screen;
   TileMap_t tileMap;
   uint8_t paletteIndex;
   uint8_t tileMapIndex;

   Menu_t menu;

   Clock_t clock;
   Input_t input;
   Physics_t physics;

   GameState_t state;
   Player_t player;

   Battle_t battle;

   uint8_t specialEnemyFlags;
   uint32_t treasureFlags;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game );
void Game_Tic( Game_t* game );
void Game_Refresh( Game_t* game );
void Game_ChangeState( Game_t *game, GameState_t newState );
void Game_SteppedOnTile( Game_t* game, uint16_t tileIndex );
void Game_ShowMessage( Game_t* game, const char* message );
void Game_WipeMessage( Game_t* game );
Bool_t Game_OnSpecialEnemyTile( Game_t* game, uint8_t specialEnemyId );
void Game_SearchMapTile( Game_t* game );
void Game_ShowMapMessage( Game_t* game, const char* message );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
