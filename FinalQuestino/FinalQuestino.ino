#include "common.h"
#include "game.h"

cGame_t g_game;

#if defined( DEBUG_SERIAL )
void cSerial_PrintLn( const char* msg )
{
   Serial.println( msg );
}
#endif

void setup()
{
#if defined( DEBUG_SERIAL )
   Serial.begin( 9600 );
#endif

   cGame_Init( &g_game );
}

void loop()
{
   cClock_StartFrame( &( g_game.clock ) );
   cGame_Tic( &g_game );
   cClock_EndFrame( &( g_game.clock ) );
}
