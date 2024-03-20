#include <time.h>

#include "random.h"

void cRandom_Seed()
{
   srand( (uint32_t)time( 0 ) );
}

uint8_t qRandom_Percent()
{
   return (uint8_t)rand() % 101;
}
