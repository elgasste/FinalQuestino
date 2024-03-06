#include "common.h"
#include "window.h"

cWindow_t g_window;

void cSerial_PrintLn( const char* msg )
{
  Serial.println( msg );
}

void setup()
{
  Serial.begin( 9600 );
  cWindow_Init( &g_window );
  cWindow_Refresh( &g_window );
}

void loop()
{
  delay( 1000 );
}
