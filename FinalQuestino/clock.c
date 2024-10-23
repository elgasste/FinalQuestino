#include "clock.h"

void Clock_Init( Clock_t* clock )
{
   clock->frameStartMicro = 0;
}

void Clock_StartFrame( Clock_t* clock )
{
   // MUFFINS
   clock->frameStartMicro = MICROS();
}

void Clock_EndFrame( Clock_t* clock )
{
   // MUFFINS
   uint32_t frameEndMicro = MICROS();
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
      DELAY_MS( ( FRAME_MICROSECONDS - elapsedMicro ) / 1000 );
   }
}
