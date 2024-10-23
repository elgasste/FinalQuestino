#include "common.h"
#include "game.h"

Game_t g_game;

#if defined( DEBUG_SERIAL )
void Serial_PrintLn( const char* msg )
{
   Serial.println( msg );
}
#endif

void setup()
{
#if defined( DEBUG_SERIAL )
   Serial.begin( 9600 );
#endif

   Game_Init( &g_game );
}

void loop()
{
   Clock_StartFrame( &( g_game.clock ) );
   Game_Tic( &g_game );
   Clock_EndFrame( &( g_game.clock ) );
}
