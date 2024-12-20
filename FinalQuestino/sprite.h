#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"

typedef struct Sprite_t
{
   uint8_t frameTextures[( SPRITE_SIZE / 2 ) * SPRITE_SIZE * SPRITE_FRAMES * 4];
   uint8_t direction;
   uint8_t currentFrame;
   float elapsedSeconds;
}
Sprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Sprite_Tic( Sprite_t* sprite );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
