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
  delay( 1000 );
}
