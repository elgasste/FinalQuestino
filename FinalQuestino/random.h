#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

void Random_Seed();
uint8_t Random_Percent();
uint8_t Random_Uint8( uint8_t min, uint8_t max );

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
