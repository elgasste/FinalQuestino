#include "tile_map.h"

void cTileMap_Init( cTileMap_t* map )
{
   uint16_t i;
   uint8_t j;
   uint8_t* tileTexture;

   for ( i = 0; i < 16; i++ )
   {
      tileTexture = map->tileTextures[i];

      for ( j = 0; j < MAP_TILE_TEXTURE_SIZE_BYTES; j++ )
      {
         tileTexture[j] = 0;
      }
   }

   for ( i = 0; i < MAP_TILE_COUNT; i++ )
   {
      map->tiles[i] = 0;
   }

   for ( i = 0; i < MAP_PORTAL_COUNT; i++ )
   {
      map->portals[i] = (uint32_t)( MAP_TILES_X * MAP_TILES_Y ) << 21; // off the map
   }

   for ( i = 0; i < MAP_TILE_ENEMY_INDEX_COUNT; i++ )
   {
      map->enemyIndexes[i] = 0;
   }

   map->stride = 1;
   map->enemyIndexCount = 0;
}
