#include "game.h"

void cGame_Init( cGame_t* game )
{
  cWindow_Init( &( game->window ) );
  game->state = cGameState_Init;
}

void cGame_Tic( cGame_t* game )
{
  if ( game->state == cGameState_Init )
  {
    cWindow_Refresh( &( game->window ) );
    game->state = cGameState_Playing;
  }
}
