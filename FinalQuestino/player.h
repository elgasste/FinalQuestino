#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "sprite.h"
#include "battle_stats.h"

typedef struct Player_t
{
   Vector2f_t position;
   Vector2f_t velocity;
   float maxVelocity;
   Sprite_t sprite;
   BattleStats_t stats;
   uint16_t experience;
   uint16_t gold;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player );
uint8_t Player_GetLevel( Player_t* player );

// data_loader.c
void Player_LoadSprite( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
