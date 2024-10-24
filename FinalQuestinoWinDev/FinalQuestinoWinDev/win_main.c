#include <stdio.h>

#include "win_common.h"
#include "game.h"

internal void FatalError( const char* message );
internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void InitOpenGL( HWND hWnd );
internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags );

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
   TIMECAPS timeCaps;
   UINT timerResolution;
   WNDCLASSA mainWindowClass = { 0 };
   DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
   RECT expectedWindowRect = { 0 };
   LONG clientPaddingRight, clientPaddingTop;
   MSG msg;

   if ( !QueryPerformanceFrequency( &( g_globals.performanceFrequency ) ) )
   {
      FatalError( "failed to query performance frequency." );
   }

   if ( timeGetDevCaps( &timeCaps, sizeof( TIMECAPS ) ) != TIMERR_NOERROR )
   {
      FatalError( "failed to set timer resolution." );
   }

   timerResolution = min( max( timeCaps.wPeriodMin, 1 ), timeCaps.wPeriodMax );
   timeBeginPeriod( timerResolution );

   mainWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   mainWindowClass.lpfnWndProc = MainWindowProc;
   mainWindowClass.hInstance = hInstance;
   mainWindowClass.lpszClassName = "mainWindowClass";

   if ( !RegisterClassA( &mainWindowClass ) )
   {
      FatalError( "failed to register window class." );
   }

   expectedWindowRect.right = SCREEN_WIDTH;
   expectedWindowRect.bottom = SCREEN_HEIGHT;

   if ( !AdjustWindowRect( &expectedWindowRect, windowStyle, 0 ) )
   {
      FatalError( "failed to adjust window rect." );
   }

   clientPaddingRight = ( expectedWindowRect.right - expectedWindowRect.left ) - SCREEN_WIDTH;
   clientPaddingTop = ( expectedWindowRect.bottom - expectedWindowRect.top ) - SCREEN_HEIGHT;

   g_globals.hWndMain = CreateWindowExA( 0,
                                         mainWindowClass.lpszClassName,
                                         "Final Questino Windows Development Tool",
                                         windowStyle,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         SCREEN_WIDTH + clientPaddingRight,
                                         SCREEN_HEIGHT + clientPaddingTop,
                                         0,
                                         0,
                                         hInstance,
                                         0 );

   if ( !g_globals.hWndMain )
   {
      FatalError( "failed to create main window." );
   }

   SetCursor( LoadCursor( 0, IDC_ARROW ) );

   InitButtonMap();
   InitOpenGL( g_globals.hWndMain );
   Game_Init( &( g_globals.game ) );
   g_globals.shutdown = False;

   while ( 1 )
   {
      Clock_StartFrame( &( g_globals.game.clock ) );
      Input_ResetState( &( g_globals.game.input ) );
      
      while ( PeekMessageA( &msg, g_globals.hWndMain, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageA( &msg );
      }

      Game_Tic( &( g_globals.game ) );
      Clock_EndFrame( &( g_globals.game.clock ) );

      if ( g_globals.shutdown )
      {
         break;
      }
   }

   return 0;
}

void Serial_PrintLn( const char* msg )
{
   OutputDebugStringA( msg );
}

internal void FatalError( const char* message )
{
   char errorMsg[STRING_SIZE_DEFAULT];
   snprintf( errorMsg, STRING_SIZE_DEFAULT, "Windows error: %s", message );
   MessageBoxA( 0, message, "Error", MB_OK | MB_ICONERROR );
   exit( 1 );
}

internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam )
{
   LRESULT result = 0;

   switch ( uMsg )
   {
      case WM_QUIT:
      case WM_CLOSE:
      case WM_DESTROY:
         g_globals.shutdown = True;
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         HandleKeyboardInput( (uint32_t)wParam, lParam );
         break;
      default:
         result = DefWindowProcA( hWnd, uMsg, wParam, lParam );
   }

   return result;
}

internal void InitButtonMap()
{
   g_globals.buttonMap[(int)Button_Left] = VK_LEFT;
   g_globals.buttonMap[(int)Button_Up] = VK_UP;
   g_globals.buttonMap[(int)Button_Right] = VK_RIGHT;
   g_globals.buttonMap[(int)Button_Down] = VK_DOWN;
   g_globals.buttonMap[(int)Button_A] = VK_SPACE;
   g_globals.buttonMap[(int)Button_B] = VK_ESCAPE;
}

internal void InitOpenGL( HWND hWnd )
{
   PIXELFORMATDESCRIPTOR desiredPixelFormat = { 0 };
   PIXELFORMATDESCRIPTOR suggestedPixelFormat;
   int suggestedPixelFormatIndex;
   HGLRC glRC;
   HDC dc = GetDC( hWnd );

   desiredPixelFormat.nSize = sizeof( desiredPixelFormat );
   desiredPixelFormat.nVersion = 1;
   desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
   desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
   desiredPixelFormat.cColorBits = 32;
   desiredPixelFormat.cAlphaBits = 8;
   desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

   suggestedPixelFormatIndex = ChoosePixelFormat( dc, &desiredPixelFormat );

   if ( !suggestedPixelFormatIndex )
   {
      FatalError( "No OpenGL pixel formats available." );
   }

   DescribePixelFormat( dc, suggestedPixelFormatIndex, sizeof( suggestedPixelFormat ), &suggestedPixelFormat );

   if ( suggestedPixelFormat.cColorBits != 32 || suggestedPixelFormat.cAlphaBits != 8 )
   {
      FatalError( "No suitable OpenGL pixel format found." );
   }

   SetPixelFormat( dc, suggestedPixelFormatIndex, &suggestedPixelFormat );
   glRC = wglCreateContext( dc );

   if ( !wglMakeCurrent( dc, glRC ) )
   {
      FatalError( "Failed to make OpenGL rendering context current." );
   }

   ReleaseDC( hWnd, dc );
}

internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags )
{
   Bool_t keyWasDown = ( flags & ( (LONG_PTR)1 << 30 ) ) != 0 ? True : False;
   Bool_t keyIsDown = ( flags & ( (LONG_PTR)1 << 31 ) ) == 0 ? True : False;
   uint32_t i;

   // ignore repeat presses
   if ( keyWasDown != keyIsDown )
   {
      if ( keyIsDown )
      {
         // ensure alt+F4 still closes the window
         if ( keyCode == VK_F4 && ( flags & ( (LONG_PTR)1 << 29 ) ) )
         {
            g_globals.shutdown = True;
            return;
         }

         for ( i = 0; i < Button_Count; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonPressed( &( g_globals.game.input ), (Button_t)i );
               break;
            }
         }
      }
      else
      {
         for ( i = 0; i < Button_Count; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonReleased( &( g_globals.game.input ), (Button_t)i );
               break;
            }
         }
      }
   }
}
