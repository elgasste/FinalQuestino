#include "random.h"
#include "win_common.h"

void Random_Seed()
{
   LARGE_INTEGER ticks;
   QueryPerformanceCounter( &ticks );

   // NOTE: normally we'd seed with time( 0 ), but this more closely resembles the kind of seeding we're doing on the Arduino.
   uint32_t micros = (uint32_t)( ( (double)( ticks.QuadPart ) / (double)( g_globals.performanceFrequency.QuadPart ) ) * (uint64_t)1000000 );
   srand( micros & (uint16_t)0xFFFF );
}

uint8_t Random_Percent()
{
   return (uint8_t)rand() % 101;
}

uint8_t Random_Uint8( uint8_t min, uint8_t max )
{
   // min and max inclusive
   return ( (uint8_t)rand() % ( ( max + 1 ) - min ) ) + min;
}
