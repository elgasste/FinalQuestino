#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"

typedef struct cTileMap_t
{
   uint8_t tileTextures[16][TILE_TEXTURE_SIZE_BYTES];

   // high 4 bits are flags, low 4 bits are tile texture index
   // flag 0001: is passable
   // flag 0010: causes damage
   // flag 0100: is encounterable
   // flag 1000: has high encounter rate
   uint8_t tiles[TILE_COUNT];

   // high 11 bits are the origin tile index
   // middle 10 bits are the destination tile map index
   // low 11 bits are the destination tile index
   uint32_t portals[PORTAL_COUNT];
}
cTileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cTileMap_Init( cTileMap_t* map );

// data_loader.c
void cTileMap_LoadTileTextures( cTileMap_t* map, uint8_t index );
void cTileMap_LoadTileMap( cTileMap_t* map, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
