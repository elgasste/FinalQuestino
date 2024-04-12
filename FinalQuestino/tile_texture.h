#if !defined( TILE_TEXTURE_H )
#define TILE_TEXTURE_H

#include "common.h"

typedef struct cTileMap_t cTileMap_t;

typedef struct cTileTexture_t
{
   uint8_t pixels[MAP_TILE_TEXTURE_SIZE_BYTES];

   // high 4 bits are flags, middle 2 bits are walking speed, low 2 bits are encounter rate
   // flag 0001: causes damage
   // flag 0010: causes super damage
   // flag 0100: reserved
   // flag 1000: reserved
   uint8_t flags;
}
cTileTexture_t;

// data_loader.c
void cTileTexture_LoadTileTextures( cTileMap_t* map );

#endif // TILE_TEXTURE_H
