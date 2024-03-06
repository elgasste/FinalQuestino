#include "game.h"

void cGame_Init( cGame_t* game )
{
  cWindow_Init( &( game->window ) );
  cTileMap_Init( &( game->tileMap ) );
  cTileMap_LoadMap( &( game->tileMap ), 0 );
  cGame_LoadTextBitFields( game );
  cPlayer_Init( &( game->player ) );
  game->state = cGameState_Init;
}

void cGame_Tic( cGame_t* game )
{
  if ( game->state == cGameState_Init )
  {
    cScreen_DrawTileMap( &( game->window.screen ), &( game->tileMap ) );
    cScreen_DrawSprite( &( game->window.screen ), &( game->player.sprite ), &( game->tileMap ), (uint16_t)game->player.position.x, (uint16_t)game->player.position.y );
    game->state = cGameState_Playing;
  }
}
