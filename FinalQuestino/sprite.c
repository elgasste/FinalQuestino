#include "sprite.h"
#include "clock.h"

void Sprite_Tic( Sprite_t* sprite, Clock_t* clock )
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
