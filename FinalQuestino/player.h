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

   // lowest 3 bits: keys
   // next 3 bits: herbs
   // next 3 bits: wings
   // next 3 bits: fairy waters
   // bit 12: fairy flute
   // bit 13: silver harp
   // bit 14: gwaelin's love
   // bit 15: stone of sunlight
   // bit 16: staff of rain
   // bit 17: erdrick's token
   // bit 18: rainbow drop
   // bit 19: ball of light
   // bits 20-31: reserved
   uint32_t items;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player );
uint8_t Player_GetLevel( Player_t* player );
uint16_t Player_CollectGold( Player_t* player, uint16_t gold );
uint16_t Player_CollectExperience( Player_t* player, uint16_t experience );
Bool_t Player_CollectItem( Player_t* player, uint8_t item );

// data_loader.c
void Player_LoadSprite( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
