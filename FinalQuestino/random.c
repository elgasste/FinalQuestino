#include <time.h>

#include "random.h"

void cRandom_Seed()
{
   srand( (uint32_t)time( 0 ) );
}

uint8_t cRandom_Percent()
{
   return (uint8_t)rand() % 101;
}

uint8_t cRandom_Uint8( uint8_t min, uint8_t max )
{
   // min and max inclusive
   return ( (uint8_t)rand() % ( ( max + 1 ) - min ) ) + min;
}
