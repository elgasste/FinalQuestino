#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "tile_texture.h"
#include "vector.h"

typedef struct cGame_t cGame_t;

typedef struct cTileMap_t
{
   cTileTexture_t tileTextures[MAP_TILE_TEXTURE_COUNT];
   uint8_t stride;

   // high 3 bits are flags, low 5 bits are tile texture index
   // flag 001: is passable
   // flag 010: is encounterable
   // flag 100: reserved
   uint8_t tiles[MAP_TILE_COUNT];
   uint16_t tileIndexCache;

   // high 11 bits are the origin tile index
   // middle 10 bits are the destination tile map index
   // low 11 bits are the destination tile index
   uint32_t portals[MAP_PORTAL_COUNT];

   cVector4u8_t enemySpecialRegion;
   uint8_t enemyIndexes[MAP_TILE_ENEMY_INDEX_COUNT];
   uint8_t enemySpecialIndexes[MAP_TILE_ENEMY_INDEX_COUNT];
}
cTileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cTileMap_Init( cTileMap_t* tileMap );
uint16_t cTileMap_GetTileIndexFromPos( cTileMap_t* tileMap, cVector2f_t* pos );

// data_loader.c
void cTileMap_LoadTileMap( cTileMap_t* map, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
