#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "enums.h"

typedef struct Sprite_t
{
   uint8_t frameTextures[( SPRITE_SIZE / 2 ) * SPRITE_SIZE * SPRITE_FRAMES * 4];
   Direction_t direction;
   uint8_t currentFrame;
   float elapsedSeconds;
   float frameSeconds;
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
