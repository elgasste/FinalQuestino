#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

typedef struct BattleStats_t
{
   uint8_t HitPoints;
   uint8_t MaxHitPoints;
   uint8_t MagicPoints;
   uint8_t MaxMagicPoints;
   uint8_t AttackPower;
   uint8_t DefensePower;
   uint8_t Agility;
}
BattleStats_t;

#endif // BATTLE_STATS_H
