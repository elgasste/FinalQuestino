#include "tile_map.h"

void cTileMap_Init( cTileMap_t* tileMap )
{
   uint16_t i;
   uint8_t j;
   cTileTexture_t* tileTexture;

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

uint16_t cTileMap_GetTileIndexFromPos( cTileMap_t* tileMap, cVector2f_t* pos )
{
   return ( ( pos->y / MAP_TILE_SIZE ) * MAP_TILES_X ) + ( pos->x / MAP_TILE_SIZE );
}
