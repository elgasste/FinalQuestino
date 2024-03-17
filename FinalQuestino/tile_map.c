#include "tile_map.h"

void cTileMap_Init( cTileMap_t* map )
{
   uint16_t i;
   uint8_t j;
   uint8_t* tileTexture;

   for ( i = 0; i < 16; i++ )
   {
      tileTexture = map->tileTextures[i];

      for ( j = 0; j < TILE_TEXTURE_SIZE_BYTES; j++ )
      {
         tileTexture[j] = 0;
      }
   }

   for ( i = 0; i < TILE_COUNT; i++ )
   {
      map->tiles[i] = 0;
   }

   for ( i = 0; i < PORTAL_COUNT; i++ )
   {
      map->portals[i] = (uint32_t)( TILES_X * TILES_Y ) << 21; // off the map
   }

   map->stride = 1;
}
