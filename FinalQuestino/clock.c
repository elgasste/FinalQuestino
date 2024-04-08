#include "clock.h"

void cClock_Init( cClock_t* clock )
{
   clock->frameStartMicro = 0;
}

void cClock_StartFrame( cClock_t* clock )
{
   clock->frameStartMicro = micros();
}

void cClock_EndFrame( cClock_t* clock )
{
   uint32_t frameEndMicro = micros();
   uint32_t elapsedMicro;

   if ( frameEndMicro < clock->frameStartMicro )
   {
      // this means the timer has overflowed (happens after ~70 mins)
      elapsedMicro = frameEndMicro + ( ULONG_MAX - clock->frameStartMicro );
   }
   else
   {
      elapsedMicro = frameEndMicro - clock->frameStartMicro;
   }

   if ( elapsedMicro <= FRAME_MICROSECONDS )
   {
      // I'd like to use delayMicroseconds here, but there are some serious
      // issues with precision. regular "delay" works much better, strangely.
      delay( ( FRAME_MICROSECONDS - elapsedMicro ) / 1000 );
   }
}
