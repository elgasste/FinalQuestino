#include "enemy.h"

void cEnemy_Generate( cEnemy_t* enemy, uint8_t index )
{
   if ( index == 0 )
   {
      snprintf( enemy->name, 16, "slime" );
      enemy->stats.HitPoints = 4;
      enemy->stats.MaxHitPoints = 4;
      enemy->stats.MagicPoints = 0;
      enemy->stats.MaxMagicPoints = 0;
      enemy->stats.AttackPower = 1;
      enemy->stats.DefensePower = 1;
      enemy->stats.Agility = 1;
   }
}
