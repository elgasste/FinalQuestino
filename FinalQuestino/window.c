#include "window.h"

void cWindow_Init( cWindow_t* window )
{
  cTileMap_Init( &( window->tileMap ) );
  cTileMap_LoadMap( &( window->tileMap ), 0 );
  cWindow_LoadTextBitFields( window );
  cScreen_Init( &( window->screen ) );
  cScreen_LoadPalette( &( window->screen ), 0 );
  cScreen_Begin( &( window->screen ) );
}

void cWindow_Refresh( cWindow_t* window )
{
  cScreen_DrawTileMap( &( window->screen ), &( window->tileMap ) );
}
