#include "tile_map.h"

void TileMap_Init( TileMap_t* tileMap )
{
   uint16_t i;
   uint8_t j;
   TileTexture_t* tileTexture;

   for ( i = 0; i < 16; i++ )
   {
      tileTexture = &( tileMap->tileTextures[i] );

      for ( j = 0; j < MAP_TILE_TEXTURE_SIZE_BYTES; j++ )
      {
         tileTexture->pixels[j] = 0;
      }
   }

   for ( i = 0; i < MAP_TILE_COUNT; i++ )
   {
      tileMap->tiles[i] = 0;
   }

   tileMap->tileIndexCache = MAP_TILES_X * MAP_TILES_Y; // off the map

   for ( i = 0; i < MAP_PORTAL_COUNT; i++ )
   {
      tileMap->portals[i] = (uint32_t)( MAP_TILES_X * MAP_TILES_Y ) << 21; // off the map
   }

   for ( i = 0; i < SPRITE_TEXTURE_SIZE_BYTES; i++ )
   {
      tileMap->spriteTexture[i] = (uint8_t)i;
   }

   tileMap->spriteCount = 0;

   for ( i = 0; i < MAP_SPRITE_COUNT; i++ )
   {
      tileMap->spriteData[i] = 0;
   }

   for ( i = 0; i < MAP_TILE_ENEMY_INDEX_COUNT; i++ )
   {
      tileMap->enemyIndexes[i] = 0;
      tileMap->enemySpecialIndexes[i] = 0;
   }

   tileMap->stride = 1;
   tileMap->enemySpecialRegion.x = MAP_TILES_X;
   tileMap->enemySpecialRegion.y = MAP_TILES_Y;
   tileMap->enemySpecialRegion.w = 0;
   tileMap->enemySpecialRegion.h = 0;
}

int16_t TileMap_GetPlayerFacingTileIndex( uint16_t playerTileIndex, uint8_t direction )
{
   uint16_t tileX = playerTileIndex % MAP_TILES_X;
   uint16_t tileY = playerTileIndex / MAP_TILES_X;

   switch ( direction )
   {
      case DIRECTION_LEFT: return ( tileX == 0 ) ? -1 : playerTileIndex - 1; break;
      case DIRECTION_RIGHT: return ( tileX >= ( MAP_TILES_X - 1 ) ) ? -1 : playerTileIndex + 1; break;
      case DIRECTION_UP: return ( tileY == 0 ) ? -1 : playerTileIndex - MAP_TILES_X; break;
      case DIRECTION_DOWN: return ( tileY >= ( MAP_TILES_Y - 1 ) ) ? -1 : playerTileIndex + MAP_TILES_X; break;
      default: return -1;
   }
}

Bool_t TileMap_TileHasSprite( TileMap_t* map, uint8_t spriteId, uint16_t tileIndex )
{
   uint8_t i;

   for ( i = 0; i < map->spriteCount; i++ )
   {
      if ( GET_SPRITE_TILE_INDEX( map->spriteData[i] ) == tileIndex )
      {
         return GET_SPRITE_INDEX( map->spriteData[i] ) == spriteId;
      }
   }

   return False;
}
