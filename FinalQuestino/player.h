#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "sprite.h"
#include "battle_stats.h"

typedef struct cPlayer_t
{
   cVector2f_t position;
   cVector2f_t velocity;
   float maxVelocity;
   cVector2f_t hitBoxSize;

   cSprite_t sprite;
   cVector2f_t spriteOffset;

   cBattleStats_t stats;
   uint16_t experience;
   uint16_t gold;
}
cPlayer_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cPlayer_Init( cPlayer_t* player );
uint8_t cPlayer_GetLevel( cPlayer_t* player );

// data_loader.c
void cPlayer_LoadSprite( cPlayer_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
