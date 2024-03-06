#include "window.h"

void cWindow_Init( cWindow_t* window )
{
  cTileMap_Init( &( window->tileMap ) );
  cTileMap_LoadPalette( &( window->tileMap ), 0 );
  cTileMap_LoadMap( &( window->tileMap ), 0 );
  cWindow_LoadTextBitFields( window );
  cSprite_Init( &( window->sprite ) );
  cScreen_Init( &( window->screen ) );
  cScreen_Begin( &( window->screen ) );
}

void cWindow_Refresh( cWindow_t* window )
{
  cScreen_DrawTileMap( &( window->screen ), &( window->tileMap ), 0, 0 );
}
