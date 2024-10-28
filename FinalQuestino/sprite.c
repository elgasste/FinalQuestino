#include "sprite.h"

void Sprite_Tic( Sprite_t* sprite )
{
   sprite->elapsedSeconds += FRAME_SECONDS;

   while ( sprite->elapsedSeconds >= SPRITE_FRAME_SECONDS )
   {
      sprite->currentFrame++;

      if ( sprite->currentFrame >= SPRITE_FRAMES )
      {
         sprite->currentFrame = 0;
      }

      sprite->elapsedSeconds -= SPRITE_FRAME_SECONDS;
   }
}
