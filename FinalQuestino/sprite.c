#include "sprite.h"
#include "clock.h"

Sprite_Init( Sprite_t* sprite )
{
   Sprite_LoadTextures( sprite );
   sprite->direction = (Direction_t)0;
   sprite->currentFrame = 0;
   sprite->elapsedSeconds = 0;
   sprite->frameSeconds = 1;
}

Sprite_Tic( Sprite_t* sprite, Clock_t* clock )
{
   sprite->elapsedSeconds += FRAME_SECONDS;

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
