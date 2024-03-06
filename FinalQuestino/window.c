#include "window.h"

void cWindow_Init( cWindow_t* window )
{
  cScreen_Init( &( window->screen ) );
  cScreen_LoadPalette( &( window->screen ), 0 );
  cScreen_Begin( &( window->screen ) );
}
