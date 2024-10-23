#if !defined( CLOCK_H )
#define CLOCK_H

#include "common.h"

typedef struct Clock_t
{
   uint32_t frameStartMicro;
}
Clock_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Clock_Init( Clock_t* clock );
void Clock_StartFrame( Clock_t* clock );
void Clock_EndFrame( Clock_t* clock );

#if defined( __cplusplus )
}
#endif

#endif // CLOCK_H
