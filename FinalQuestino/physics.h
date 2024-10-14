#if !defined( PHYSICS_H )
#define PHYSICS_H

#include "common.h"

typedef struct Game_t Game_t;

typedef struct Physics_t
{
   uint8_t spriteFrameCache;
   uint16_t tileIndexCache;
}
Physics_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Physics_Init( Physics_t* physics );
void Physics_Tic( Game_t* game );
void Physics_UpdateTileIndexCache( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // PHYSICS_H
