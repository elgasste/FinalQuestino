#include "random.h"

void Random_Seed()
{
   // MUFFINS: what is this?
   srand( DELAY_MS() & (uint16_t)0xFFFF );
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
