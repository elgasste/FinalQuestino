#include "game.h"

#define COLLISION_PADDING 0.001f

void cPhysics_Init( cPhysics_t* physics )
{
  physics->spriteFrameCache = 0;
}

void cPhysics_MovePlayer( cGame_t* game )
{
  cVector2f_t newPos;
  cPlayer_t* player = &( game->player );
  uint8_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile;

  newPos.x = player->position.x + ( player->velocity.x * game->clock.frameSeconds );
  newPos.y = player->position.y + ( player->velocity.y * game->clock.frameSeconds );

  // clip to screen boundaries
  if ( newPos.x < 0 )
  {
    newPos.x = COLLISION_PADDING;
  }
  else if ( ( newPos.x + player->hitBoxSize.x ) >= ( TILES_X * TILE_SIZE ) )
  {
    newPos.x = ( TILES_X * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_PADDING;
  }
  if ( newPos.y < 0 )
  {
    newPos.y = COLLISION_PADDING;
  }
  else if ( ( newPos.y + player->hitBoxSize.y ) >= ( TILES_Y * TILE_SIZE ) )
  {
    newPos.y = ( TILES_Y * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_PADDING;
  }

  // clip to unpassable horizontal tiles
  if ( newPos.x != player->position.x )
  {
    tileRowStartIndex = player->position.y / TILE_SIZE;
    tileRowEndIndex = ( player->position.y + player->hitBoxSize.y ) / TILE_SIZE;

    if ( newPos.x < player->position.x )
    {
      // moving left, check leftward tiles
      col = newPos.x / TILE_SIZE;

      for( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          newPos.x = ( ( col + 1 ) * TILE_SIZE );
          break;
        }
      }
    }
    else
    {
      // moving right, check rightward tiles
      col = ( newPos.x + player->hitBoxSize.x ) / TILE_SIZE;

      for( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          newPos.x = ( col * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_PADDING;
          break;
        }
      }
    }
  }

  // clip to unpassable vertical tiles
  if ( newPos.y != player->position.y )
  {
    tileColStartIndex = player->position.x / TILE_SIZE;
    tileColEndIndex = ( player->position.x + player->hitBoxSize.x ) / TILE_SIZE;

    if ( newPos.y < player->position.y )
    {
      // moving up, check upward tiles
      row = newPos.y / TILE_SIZE;

      for( col = tileColStartIndex; col <= tileColEndIndex; col++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          newPos.y = ( ( row + 1 ) * TILE_SIZE );
          break;
        }
      }
    }
    else
    {
      // moving down, check downward tiles
      row = ( newPos.y + player->hitBoxSize.y ) / TILE_SIZE;

      for( col = tileColStartIndex; col <= tileColEndIndex; col++ )
      {
        tile = game->tileMap.tiles[col + ( row * TILES_X )];

        if ( !( tile & TILE_PASSABLE_FLAG ) )
        {
          newPos.y = ( row * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_PADDING;
          break;
        }
      }
    }
  }

  if ( newPos.x != player->position.x || newPos.y != player->position.y ||
       player->sprite.currentFrame != game->physics.spriteFrameCache )
  {
    cScreen_WipeSprite( &( game->screen ), &( game->tileMap ), player->position.x + player->spriteOffset.x, player->position.y + player->spriteOffset.y );
    game->physics.spriteFrameCache = player->sprite.currentFrame;
  }

  player->position.x = newPos.x;
  player->position.y = newPos.y;
  cScreen_DrawSprite( &( game->screen ), &( player->sprite ), &( game->tileMap ), player->position.x + player->spriteOffset.x, player->position.y + player->spriteOffset.y );

  player->velocity.x = 0;
  player->velocity.y = 0;
}
