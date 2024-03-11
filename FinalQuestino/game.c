#include "game.h"

void cGame_Init( cGame_t* game )
{
  game->paletteIndex = 0;
  game->tileTexturesIndex = 0;
  game->tilesIndex = 23;

  cScreen_Init( &( game->screen ) );
  cScreen_LoadPalette( &( game->screen ), game->paletteIndex );
  cScreen_Begin( &( game->screen ) );

  cClock_Init( &( game->clock ) );
  cInput_Init( &( game->input ) );

  cTileMap_Init( &( game->tileMap ) );
  cTileMap_LoadTileTextures( &( game->tileMap ), game->tileTexturesIndex );
  cTileMap_LoadTiles( &( game->tileMap ), game->tilesIndex );

  cPlayer_Init( &( game->player ) );
  game->player.sprite.direction = cDirection_Down;
  game->player.sprite.frameSeconds = 0.2f;
  game->player.position.x = TILE_SIZE * 10;
  game->player.position.y = TILE_SIZE * 6;
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
