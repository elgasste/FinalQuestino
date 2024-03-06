#include "game.h"

void cGame_Init( cGame_t* game )
{
  cScreen_Init( &( game->screen ) );
  cScreen_LoadPalette( &( game->screen ), 0 );
  cScreen_Begin( &( game->screen ) );

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
    cScreen_DrawTileMap( &( game->screen ), &( game->tileMap ) );
    cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ), &( game->player.position ) );
    game->state = cGameState_Playing;
  }
}
