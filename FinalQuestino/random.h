#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

void cRandom_Seed();
uint8_t cRandom_Percent();

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
