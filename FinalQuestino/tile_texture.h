#if !defined( TILE_TEXTURE_H )
#define TILE_TEXTURE_H

#include "common.h"

typedef struct cTileTexture_t
{
   uint8_t pixels[MAP_TILE_TEXTURE_SIZE_BYTES];
   uint8_t flags;
}
cTileTexture_t;

#endif // TILE_TEXTURE_H
