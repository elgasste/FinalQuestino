#include "game.h"

void cGame_Init( cGame_t* game )
{
  game->paletteIndex = 0;
  game->tileTexturesIndex = 0;
  game->tileMapIndex = 23;

  cScreen_Init( &( game->screen ) );
  cScreen_LoadPalette( &( game->screen ), game->paletteIndex );
  cScreen_Begin( &( game->screen ) );

  cClock_Init( &( game->clock ) );
  cInput_Init( &( game->input ) );

  cTileMap_Init( &( game->tileMap ) );
  cTileMap_LoadTileTextures( &( game->tileMap ), game->tileTexturesIndex );
  cTileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );

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

void cGame_Refresh( cGame_t* game )
{
  if ( game->state == cGameState_Playing )
  {
    cTileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );
    cScreen_DrawTileMap( &( game->screen ), &( game->tileMap ) );
    cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                        game->player.position.x + game->player.spriteOffset.x,
                        game->player.position.y + game->player.spriteOffset.y );
    cPhysics_UpdateTileIndexCache( game );
  }
}

void cGame_SteppedOnTile( cGame_t* game, uint16_t tileIndex )
{
  uint8_t i;
  uint8_t tile = game->tileMap.tiles[tileIndex];
  uint16_t newTileIndex, newTileX, newTileY;

  for ( i = 0; i < PORTAL_COUNT; i++ )
  {
    if ( ( game->tileMap.portals[i] >> 21 ) == tileIndex )
    {
      game->tileMapIndex = ( game->tileMap.portals[i] >> 11 ) & 0x3FF;
      newTileIndex = game->tileMap.portals[i] & 0x7FF;
      newTileY = newTileIndex / TILES_X;
      newTileX = newTileIndex - ( newTileY * TILES_X );
      game->player.position.x = ( newTileX * TILE_SIZE ) + ( ( TILE_SIZE - game->player.hitBoxSize.x ) / 2 );
      game->player.position.y = ( newTileY * TILE_SIZE ) + ( TILE_SIZE - game->player.hitBoxSize.y ) - COLLISION_PADDING;
      cGame_Refresh( game );
      return;
    }
  }

  if ( tile & TILE_DAMAGE_FLAG )
  {
    // TODO: inflict damage
  }

  if ( tile & TILE_ENCOUNTERABLE_FLAG )
  {
    // TODO: roll for encounter
  }
}
