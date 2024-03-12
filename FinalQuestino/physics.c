#include "game.h"

static void cPhysics_RefreshFromScreenSwap( cGame_t* game );

void cPhysics_Init( cPhysics_t* physics )
{
   physics->spriteFrameCache = 0;
   physics->tileIndexCache = TILES_X * TILES_Y; // off the map
}

void cPhysics_MovePlayer( cGame_t* game )
{
   cVector2f_t newPos;
   cPlayer_t* player = &( game->player );
   uint8_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile;
   cBool_t posChanged;

   newPos.x = player->position.x + ( player->velocity.x * game->clock.frameSeconds );
   newPos.y = player->position.y + ( player->velocity.y * game->clock.frameSeconds );

   // check screen boundaries
   if ( newPos.x < 0 )
   {
      game->tileMapIndex--;
      player->position.x = ( TILES_X * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }
   else if ( ( newPos.x + player->hitBoxSize.x ) >= ( TILES_X * TILE_SIZE ) )
   {
      game->tileMapIndex++;
      player->position.x = COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }

   if ( newPos.y < 0 )
   {
      game->tileMapIndex -= SCREENS_X;
      player->position.y = ( TILES_Y * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }
   else if ( ( newPos.y + player->hitBoxSize.y ) >= ( TILES_Y * TILE_SIZE ) )
   {
      game->tileMapIndex += SCREENS_X;
      player->position.y = COLLISION_PADDING;
      cGame_Refresh( game );
      return;
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

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
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

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
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

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
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

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
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

   posChanged = newPos.x != player->position.x || newPos.y != player->position.y;

   if ( posChanged || player->sprite.currentFrame != game->physics.spriteFrameCache )
   {
      cScreen_WipeSprite( &( game->screen ), &( game->tileMap ),
                          player->position.x + player->spriteOffset.x,
                          player->position.y + player->spriteOffset.y );
      game->physics.spriteFrameCache = player->sprite.currentFrame;
   }

   player->position.x = newPos.x;
   player->position.y = newPos.y;
   cScreen_DrawSprite( &( game->screen ), &( player->sprite ), &( game->tileMap ),
                       player->position.x + player->spriteOffset.x,
                       player->position.y + player->spriteOffset.y );

   if ( posChanged )
   {
      cPhysics_UpdateTileIndexCache( game );
   }

   player->velocity.x = 0;
   player->velocity.y = 0;
}

void cPhysics_UpdateTileIndexCache( cGame_t* game )
{
   uint16_t centerX = game->player.position.x + ( game->player.hitBoxSize.x / 2 );
   uint16_t centerY = game->player.position.y + ( game->player.hitBoxSize.y / 2 );
   uint16_t newTileIndex = ( centerX / TILE_SIZE ) + ( ( centerY / TILE_SIZE ) * TILES_X );

   if ( newTileIndex != game->physics.tileIndexCache )
   {
      game->physics.tileIndexCache = newTileIndex;
      cGame_SteppedOnTile( game, newTileIndex );
   }
}
