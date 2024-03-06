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
}
