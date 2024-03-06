#include "game.h"

void cGame_Init( cGame_t* game )
{
  cWindow_Init( &( game->window ) );
  // TODO: this probably doesn't belong here, it should only happen when switching maps
  cWindow_Refresh( &( game->window ) );
}
