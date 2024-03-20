#include "player.h"

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
}
