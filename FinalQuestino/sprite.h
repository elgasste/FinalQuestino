#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "enums.h"

typedef struct cClock_t cClock_t;

typedef struct cSprite_t
{
   uint8_t frameTextures[( SPRITE_SIZE / 2 ) * SPRITE_SIZE * SPRITE_FRAMES * 4];
   cDirection_t direction;
   uint8_t currentFrame;
   float elapsedSeconds;
   float frameSeconds;
}
cSprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cSprite_Init( cSprite_t* sprite );
void cSprite_Tic( cSprite_t* sprite, cClock_t* clock );

// data_loader.c
void cSprite_LoadTextures( cSprite_t* sprite );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
