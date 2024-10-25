#include "clock.h"
#include "win_common.h"

internal uint32_t Clock_GetMicros();

void Clock_Init( Clock_t* clock )
{
   clock->frameStartMicro = 0;
}

void Clock_StartFrame( Clock_t* clock )
{
   clock->frameStartMicro = Clock_GetMicros();
}

void Clock_EndFrame( Clock_t* clock )
{
   uint32_t frameEndMicro = Clock_GetMicros();
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
      DELAY_MS( ( FRAME_MICROSECONDS - elapsedMicro ) / 1000 );
   }
}

internal uint32_t Clock_GetMicros()
{
   LARGE_INTEGER ticks;
   QueryPerformanceCounter( &ticks );
   return (uint32_t)( ( (double)( ticks.QuadPart ) / (double)( g_globals.performanceFrequency.QuadPart ) ) * (uint64_t)1000000 );
}
