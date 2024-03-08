#if !defined( PHYSICS_H )
#define PHYSICS_H

#include "common.h"

typedef struct cGame_t cGame_t;

typedef struct cPhysics_t
{
  uint8_t spriteFrameCache;
}
cPhysics_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cPhysics_Init( cPhysics_t* physics );
void cPhysics_MovePlayer( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // PHYSICS_H