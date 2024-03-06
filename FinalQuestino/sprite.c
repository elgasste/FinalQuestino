#include "sprite.h"

cSprite_Init( cSprite_t* sprite )
{
  cSprite_LoadTextures( sprite );
  sprite->direction = (cDirection_t)0;
  sprite->currentFrame = 0;
  sprite->elapsedSeconds = 0;
}

cSprite_Tic( cSprite_t* sprite )
{
  // TODO: bring in the clock, and use it to tic the sprite
}
