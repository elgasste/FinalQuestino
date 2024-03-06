#include "common.h"
#include "game.h"

cGame_t g_game;

void cSerial_PrintLn( const char* msg )
{
  Serial.println( msg );
}

void setup()
{
  Serial.begin( 9600 );
  cGame_Init( &g_game );
}

void loop()
{
  cClock_StartFrame( &( g_game.clock ) );

  // TODO: update and render

  cClock_EndFrame( &( g_game.clock ) );
}
