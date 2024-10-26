#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "tile_texture.h"
#include "vector.h"

typedef struct Game_t Game_t;

typedef struct TileMap_t
{
   TileTexture_t tileTextures[MAP_TILE_TEXTURE_COUNT];
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

   uint8_t spriteTexture[SPRITE_TEXTURE_SIZE_BYTES];
   uint8_t spriteCount;

   // low 9 bits are the tile index
   // middle 4 bits are the sprite index
   // high 3 bits are flags
   // flag 001: is passable
   // flag 010: reserved
   // flag 100: reserved
   uint16_t spriteData[MAP_SPRITE_COUNT];

   Vector4u8_t enemySpecialRegion;
   uint8_t enemyIndexes[MAP_TILE_ENEMY_INDEX_COUNT];
   uint8_t enemySpecialIndexes[MAP_TILE_ENEMY_INDEX_COUNT];
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap );

// data_loader.c
void TileMap_LoadTileMap( TileMap_t* map, uint8_t index );
void TileMap_LoadSprite( TileMap_t* map, uint8_t index );
uint32_t TileMap_GetTreasureFlag( uint8_t tileMapIndex, uint16_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
