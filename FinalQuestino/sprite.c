#include "sprite.h"
#include "clock.h"

cSprite_Init( cSprite_t* sprite )
{
  cSprite_LoadTextures( sprite );
  sprite->direction = (cDirection_t)0;
  sprite->currentFrame = 0;
  sprite->elapsedSeconds = 0;
  sprite->frameSeconds = 1;
}

cSprite_Tic( cSprite_t* sprite, cClock_t* clock )
{
  sprite->elapsedSeconds += clock->frameSeconds;

  while ( sprite->elapsedSeconds >= sprite->frameSeconds )
  {
    sprite->currentFrame++;

    if ( sprite->currentFrame >= SPRITE_FRAMES )
    {
      sprite->currentFrame = 0;
    }

    sprite->elapsedSeconds -= sprite->frameSeconds;
  }
}
