#if !defined( WINDOW_H )
#define WINDOW_H

#include "common.h"
#include "screen.h"

typedef struct cWindow_t
{
  cScreen_t screen;
}
cWindow_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cWindow_Init( cWindow_t* window );

#if defined( __cplusplus )
}
#endif

#endif // WINDOW_H
