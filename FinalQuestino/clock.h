#if !defined( CLOCK_H )
#define CLOCK_H

#include "common.h"

typedef struct cClock_t
{
   uint32_t frameStartMicro;
}
cClock_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cClock_Init( cClock_t* clock );
void cClock_StartFrame( cClock_t* clock );
void cClock_EndFrame( cClock_t* clock );

#if defined( __cplusplus )
}
#endif

#endif // CLOCK_H
