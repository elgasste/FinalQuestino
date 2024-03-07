#include "game.h"

void cGame_Init( cGame_t* game )
{
  cScreen_Init( &( game->screen ) );
  cScreen_LoadPalette( &( game->screen ), 0 );
  cScreen_Begin( &( game->screen ) );

  cClock_Init( &( game->clock ) );
  cInput_Init( &( game->input ) );

  cTileMap_Init( &( game->tileMap ) );
  cTileMap_LoadMap( &( game->tileMap ), 0 );
  cGame_LoadTextBitFields( game );

  cPlayer_Init( &( game->player ) );
  game->player.sprite.direction = cDirection_Down;
  game->player.sprite.frameSeconds = 0.2f;
  game->player.position.x = ( TILES_X * TILE_SIZE ) / 2;
  game->player.position.y = ( TILES_Y * TILE_SIZE ) / 2;
  game->player.hitBoxSize.x = 12;
  game->player.hitBoxSize.y = 12;
  game->player.spriteOffset.x = -2;
  game->player.spriteOffset.y = -4;

  game->state = cGameState_Init;
}

void cGame_Tic( cGame_t* game )
{
  cInput_Read( &( game->input ) );
  cInput_Handle( game );

  switch( game->state )
  {
    case cGameState_Init:
      cScreen_DrawTileMap( &( game->screen ), &( game->tileMap ) );
      game->state = cGameState_Playing;
      break;
    case cGameState_Playing:
      cPhysics_MovePlayer( game );
      break;
  }
}
