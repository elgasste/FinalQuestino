#include "game.h"

#define COLLISION_PADDING 0.001f

void cPhysics_Init( cPhysics_t* physics )
{
  physics->spriteFrameCache = 0;
}

void cPhysics_MovePlayer( cGame_t* game )
{
  cVector2f_t prevPlayerPos;
  cPlayer_t* player = &( game->player );
  uint8_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile;

  prevPlayerPos.x = player->position.x;
  prevPlayerPos.y = player->position.y;
  player->position.x += ( player->velocity.x * game->clock.frameSeconds );
  player->position.y += ( player->velocity.y * game->clock.frameSeconds );

  // clip to screen boundaries
  if ( player->position.x < 0 )
  {
    player->position.x = 0;
  }
  else if ( ( player->position.x + player->hitBoxSize.x ) >= ( TILES_X * TILE_SIZE ) )
  {
    player->position.x = ( TILES_X * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_PADDING;
  }
  if ( player->position.y < 0 )
  {
    player->position.y = 0;
  }
  else if ( ( player->position.y + player->hitBoxSize.y ) >= ( TILES_Y * TILE_SIZE ) )
  {
    player->position.y = ( TILES_Y * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_PADDING;
  }

  // clip to unpassable horizontal tiles
  if ( player->position.x != prevPlayerPos.x )
  {
    tileRowStartIndex = player->position.y / TILE_SIZE;
    tileRowEndIndex = ( player->position.y + player->hitBoxSize.y ) / TILE_SIZE;

    if ( player->position.x < prevPlayerPos.x )
    {
      // moving left, check leftward tiles
      col = player->position.x / TILE_SIZE;

      for( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          player->position.x = ( ( col + 1 ) * TILE_SIZE );
          break;
        }
      }
    }
    else
    {
      // moving right, check rightward tiles
      col = ( player->position.x + player->hitBoxSize.x ) / TILE_SIZE;

      for( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
      {
        tile = game->tileMap.tiles[ col + ( row * TILES_X ) ];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          player->position.x = ( col * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_PADDING;
          break;
        }
      }
    }
  }

  // clip to unpassable vertical tiles
  if ( player->position.y != prevPlayerPos.y )
  {
    tileColStartIndex = player->position.x / TILE_SIZE;
    tileColEndIndex = ( player->position.x + player->hitBoxSize.x ) / TILE_SIZE;

    if ( player->position.y < prevPlayerPos.y )
    {
      // moving up, check upward tiles
      row = player->position.y / TILE_SIZE;

      for( col = tileColStartIndex; col <= tileColEndIndex; col++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          player->position.y = ( ( row + 1 ) * TILE_SIZE );
          break;
        }
      }
    }
    else
    {
      // moving down, check downward tiles
      row = ( player->position.y + player->hitBoxSize.y ) / TILE_SIZE;

      for( col = tileColStartIndex; col <= tileColEndIndex; col++ )
      {
        tile = game->tileMap.tiles[ col + ( row * TILES_X ) ];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          player->position.y = ( row * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_PADDING;
          break;
        }
      }
    }
  }

  player->velocity.x = 0;
  player->velocity.y = 0;

  if ( prevPlayerPos.x != player->position.x || prevPlayerPos.y != player->position.y ||
       player->sprite.currentFrame != game->physics.spriteFrameCache )
  {
    cScreen_WipeSprite( &( game->screen ), &( game->tileMap ),
                        prevPlayerPos.x + player->spriteOffset.x,
                        prevPlayerPos.y + player->spriteOffset.y );
    game->physics.spriteFrameCache = player->sprite.currentFrame;
  }

  cScreen_DrawSprite( &( game->screen ), &( player->sprite ), &( game->tileMap ),
                      player->position.x + player->spriteOffset.x,
                      player->position.y + player->spriteOffset.y );
}
