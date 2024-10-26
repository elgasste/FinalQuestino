#include "game.h"

internal Bool_t Physics_TileHasImpassableSprite( TileMap_t* map, uint16_t tileIndex );

void Physics_Init( Physics_t* physics )
{
   physics->spriteFrameCache = 0;
   physics->tileIndexCache = MAP_TILES_X * MAP_TILES_Y; // off the map
}

void Physics_Tic( Game_t* game )
{
   Vector2f_t newPos;
   Player_t* player = &( game->player );
   uint8_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile;
   uint16_t tileIndex;
   Bool_t posChanged;

   newPos.x = player->position.x + ( player->velocity.x * FRAME_SECONDS );
   newPos.y = player->position.y + ( player->velocity.y * FRAME_SECONDS );

   // check screen boundaries
   if ( newPos.x < 0 )
   {
      game->tileMapIndex--;
      player->position.x = ( MAP_TILES_X * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
      Game_Refresh( game );
      return;
   }
   else if ( ( newPos.x + PLAYER_HITBOX_SIZE ) >= ( MAP_TILES_X * MAP_TILE_SIZE ) )
   {
      game->tileMapIndex++;
      player->position.x = COLLISION_PADDING;
      Game_Refresh( game );
      return;
   }

   if ( newPos.y < 0 )
   {
      game->tileMapIndex -= game->tileMap.stride;
      player->position.y = ( MAP_TILES_Y * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
      Game_Refresh( game );
      return;
   }
   else if ( ( newPos.y + PLAYER_HITBOX_SIZE ) >= ( MAP_TILES_Y * MAP_TILE_SIZE ) )
   {
      game->tileMapIndex += game->tileMap.stride;
      player->position.y = COLLISION_PADDING;
      Game_Refresh( game );
      return;
   }

   // clip to unpassable horizontal tiles
   if ( newPos.x != player->position.x )
   {
      tileRowStartIndex = ( uint8_t )( player->position.y / MAP_TILE_SIZE );
      tileRowEndIndex = ( uint8_t )( ( player->position.y + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE );

      if ( newPos.x < player->position.x )
      {
         // moving left, check leftward tiles
         col = (uint8_t)( newPos.x / MAP_TILE_SIZE );

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !GET_TILE_PASSABLE( tile ) || Physics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.x = (float)( ( ( col + 1 ) * MAP_TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving right, check rightward tiles
         col = (uint8_t )( ( newPos.x + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE );

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !GET_TILE_PASSABLE( tile ) || Physics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.x = ( col * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
               break;
            }
         }
      }
   }

   // clip to unpassable vertical tiles
   if ( newPos.y != player->position.y )
   {
      tileColStartIndex = ( uint8_t )( player->position.x / MAP_TILE_SIZE );
      tileColEndIndex = (uint8_t)( ( player->position.x + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE );

      if ( newPos.y < player->position.y )
      {
         // moving up, check upward tiles
         row = (uint8_t)( newPos.y / MAP_TILE_SIZE );

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !GET_TILE_PASSABLE( tile ) || Physics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.y = (float)( ( ( row + 1 ) * MAP_TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving down, check downward tiles
         row = (uint8_t)( ( newPos.y + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE );

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !GET_TILE_PASSABLE( tile ) || Physics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.y = ( row * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
               break;
            }
         }
      }
   }

   posChanged = newPos.x != player->position.x || newPos.y != player->position.y;

   if ( posChanged || player->sprite.currentFrame != game->physics.spriteFrameCache )
   {
      Screen_WipePlayer( game );
      game->physics.spriteFrameCache = player->sprite.currentFrame;
   }

   player->position.x = newPos.x;
   player->position.y = newPos.y;
   Screen_DrawPlayer( game );

   if ( posChanged )
   {
      Physics_UpdateTileIndexCache( game );
   }

   player->velocity.x = 0;
   player->velocity.y = 0;
}

void Physics_UpdateTileIndexCache( Game_t* game )
{
   uint16_t centerX = (uint16_t)( game->player.position.x + ( PLAYER_HITBOX_SIZE / 2 ) );
   uint16_t centerY = (uint16_t)( game->player.position.y + ( PLAYER_HITBOX_SIZE / 2 ) );
   uint16_t newTileIndex = ( centerX / MAP_TILE_SIZE ) + ( ( centerY / MAP_TILE_SIZE ) * MAP_TILES_X );

   if ( newTileIndex != game->physics.tileIndexCache )
   {
      game->physics.tileIndexCache = newTileIndex;
      Game_SteppedOnTile( game, newTileIndex );
   }
}

internal Bool_t Physics_TileHasImpassableSprite( TileMap_t* map, uint16_t tileIndex )
{
   uint8_t i;

   for ( i = 0; i < map->spriteCount; i++ )
   {
      if ( (uint16_t)( GET_SPRITE_TILE_INDEX( map->spriteData[i] ) ) == tileIndex )
      {
         return GET_SPRITE_PASSABLE( map->spriteData[i] ) ? False : True;
      }
   }

   return False;
}
