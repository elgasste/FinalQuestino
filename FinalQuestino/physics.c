#include "game.h"

static void cPhysics_RefreshFromScreenSwap( cGame_t* game );
static cBool_t cPhysics_TileHasImpassableSprite( cTileMap_t* map, uint16_t tileIndex );

void cPhysics_Init( cPhysics_t* physics )
{
   physics->spriteFrameCache = 0;
   physics->tileIndexCache = MAP_TILES_X * MAP_TILES_Y; // off the map
}

void cPhysics_Tic( cGame_t* game )
{
   cVector2f_t newPos;
   cPlayer_t* player = &( game->player );
   uint8_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile;
   uint16_t tileIndex;
   cBool_t posChanged;

   newPos.x = player->position.x + ( player->velocity.x * FRAME_SECONDS );
   newPos.y = player->position.y + ( player->velocity.y * FRAME_SECONDS );

   // check screen boundaries
   if ( newPos.x < 0 )
   {
      game->tileMapIndex--;
      player->position.x = ( MAP_TILES_X * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }
   else if ( ( newPos.x + PLAYER_HITBOX_SIZE ) >= ( MAP_TILES_X * MAP_TILE_SIZE ) )
   {
      game->tileMapIndex++;
      player->position.x = COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }

   if ( newPos.y < 0 )
   {
      game->tileMapIndex -= game->tileMap.stride;
      player->position.y = ( MAP_TILES_Y * MAP_TILE_SIZE ) - PLAYER_HITBOX_SIZE - COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }
   else if ( ( newPos.y + PLAYER_HITBOX_SIZE ) >= ( MAP_TILES_Y * MAP_TILE_SIZE ) )
   {
      game->tileMapIndex += game->tileMap.stride;
      player->position.y = COLLISION_PADDING;
      cGame_Refresh( game );
      return;
   }

   // clip to unpassable horizontal tiles
   if ( newPos.x != player->position.x )
   {
      tileRowStartIndex = player->position.y / MAP_TILE_SIZE;
      tileRowEndIndex = ( player->position.y + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE;

      if ( newPos.x < player->position.x )
      {
         // moving left, check leftward tiles
         col = newPos.x / MAP_TILE_SIZE;

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !( tile & MAP_TILE_FLAG_PASSABLE ) || cPhysics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.x = ( ( col + 1 ) * MAP_TILE_SIZE );
               break;
            }
         }
      }
      else
      {
         // moving right, check rightward tiles
         col = ( newPos.x + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE;

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !( tile & MAP_TILE_FLAG_PASSABLE ) || cPhysics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
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
      tileColStartIndex = player->position.x / MAP_TILE_SIZE;
      tileColEndIndex = ( player->position.x + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE;

      if ( newPos.y < player->position.y )
      {
         // moving up, check upward tiles
         row = newPos.y / MAP_TILE_SIZE;

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !( tile & MAP_TILE_FLAG_PASSABLE ) || cPhysics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
            {
               newPos.y = ( ( row + 1 ) * MAP_TILE_SIZE );
               break;
            }
         }
      }
      else
      {
         // moving down, check downward tiles
         row = ( newPos.y + PLAYER_HITBOX_SIZE ) / MAP_TILE_SIZE;

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
         {
            tileIndex = col + ( row * MAP_TILES_X );
            tile = game->tileMap.tiles[tileIndex];

            if ( !( tile & MAP_TILE_FLAG_PASSABLE ) || cPhysics_TileHasImpassableSprite( &( game->tileMap ), tileIndex ) )
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
      cScreen_WipePlayer( game );
      game->physics.spriteFrameCache = player->sprite.currentFrame;
   }

   player->position.x = newPos.x;
   player->position.y = newPos.y;
   cScreen_DrawPlayer( game );

   if ( posChanged )
   {
      cPhysics_UpdateTileIndexCache( game );
   }

   player->velocity.x = 0;
   player->velocity.y = 0;
}

void cPhysics_UpdateTileIndexCache( cGame_t* game )
{
   uint16_t centerX = game->player.position.x + ( PLAYER_HITBOX_SIZE / 2 );
   uint16_t centerY = game->player.position.y + ( PLAYER_HITBOX_SIZE / 2 );
   uint16_t newTileIndex = ( centerX / MAP_TILE_SIZE ) + ( ( centerY / MAP_TILE_SIZE ) * MAP_TILES_X );

   if ( newTileIndex != game->physics.tileIndexCache )
   {
      game->physics.tileIndexCache = newTileIndex;
      cGame_SteppedOnTile( game, newTileIndex );
   }
}

static cBool_t cPhysics_TileHasImpassableSprite( cTileMap_t* map, uint16_t tileIndex )
{
   uint8_t i;

   for ( i = 0; i < map->spriteCount; i++ )
   {
      if ( (uint16_t)( map->spriteData[i] & 0x1FF ) == tileIndex )
      {
         return ( ( map->spriteData[i] >> 13 ) & 0x1 ) ? cFalse : cTrue;
      }
   }

   return cFalse;
}
