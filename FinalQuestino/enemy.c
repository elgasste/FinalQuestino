#include "enemy.h"

void cEnemy_Generate( cEnemy_t* enemy )
{
   snprintf( enemy->name, 16, "Slime" );
   enemy->stats.HitPoints = 4;
   enemy->stats.MaxHitPoints = 4;
   enemy->stats.MagicPoints = 0;
   enemy->stats.MaxMagicPoints = 0;
   enemy->stats.AttackPower = 1;
   enemy->stats.DefensePower = 1;
   enemy->stats.Agility = 1;
}
