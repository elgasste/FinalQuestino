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
  // TODO: we don't have a clock yet, but when we do, it should be a function parameter here
}
