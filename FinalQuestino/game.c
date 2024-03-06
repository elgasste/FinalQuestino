#include "game.h"

void cGame_Init( cGame_t* game )
{
  cWindow_Init( &( game->window ) );
  cWindow_Refresh( &( game->window ) );
}
