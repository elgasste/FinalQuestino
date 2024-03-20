#include "player.h"

static uint16_t experienceChart[30] = {
   0, 7, 23, 47, 110, 220, 450, 800, 1300, 2000, 2900, 4000, 5500,
   7500, 10000, 13000, 16000, 19000, 22000, 26000, 30000, 34000,
   38000, 42000, 46000, 50000, 54000, 58000, 62000, 65535
};

void cPlayer_Init( cPlayer_t* player )
{
   cPlayer_LoadSprite( player );

   player->position.x = 0;
   player->position.y = 0;
   player->velocity.x = 0;
   player->velocity.y = 0;
   player->maxVelocity.x = PLAYER_MAX_VELOCITY;
   player->maxVelocity.y = PLAYER_MAX_VELOCITY;
   player->hitBoxSize.x = 0;
   player->hitBoxSize.y = 0;
   player->spriteOffset.x = 0;
   player->spriteOffset.y = 0;

   player->stats.HitPoints = 12;
   player->stats.MaxHitPoints = 12;
   player->stats.MagicPoints = 0;
   player->stats.MaxMagicPoints = 0;
   player->stats.AttackPower = 2;
   player->stats.DefensePower = 2;
   player->stats.Agility = 2;

   player->experience = 0;
}

uint8_t cPlayer_GetLevel( cPlayer_t* player )
{
   uint8_t i;

   for ( i = 0; i < 30; i++ )
   {
      if ( player->experience <= experienceChart[i] )
      {
         return i + 1;
      }
   }

   return 0;
}
