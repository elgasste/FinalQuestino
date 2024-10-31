#include <stdio.h>

#include "win_common.h"
#include "game.h"
#include "win_pixel_buffer.h"
#include "screen.h"

internal void FatalError( const char* message );
internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void InitBattleStartRects();
internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags );
internal void RenderScreen();
internal void BattleStartAnimationTic();
internal void BattleAttackAnimationTic();

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
   TIMECAPS timeCaps;
   UINT timerResolution;
   WNDCLASSA mainWindowClass = { 0 };
   DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
   RECT expectedWindowRect = { 0 };
   LONG clientPaddingRight, clientPaddingTop;
   MSG msg;

   UNUSED_PARAM( hPrevInstance );
   UNUSED_PARAM( lpCmdLine );
   UNUSED_PARAM( nCmdShow );

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
                                         (int)( SCREEN_WIDTH * GRAPHICS_SCALE ) + clientPaddingRight,
                                         (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ) + clientPaddingTop,
                                         0,
                                         0,
                                         hInstance,
                                         0 );

   if ( !g_globals.hWndMain )
   {
      FatalError( "failed to create main window." );
   }

   SetCursor( LoadCursor( 0, IDC_ARROW ) );

   WinPixelBuffer_Init( &( g_globals.screenBuffer ), SCREEN_WIDTH, SCREEN_HEIGHT );
   g_globals.bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   g_globals.bmpInfo.bmiHeader.biWidth = g_globals.screenBuffer.w;
   g_globals.bmpInfo.bmiHeader.biHeight = -(LONG)( g_globals.screenBuffer.h );
   g_globals.bmpInfo.bmiHeader.biPlanes = 1;
   g_globals.bmpInfo.bmiHeader.biBitCount = 32;
   g_globals.bmpInfo.bmiHeader.biCompression = BI_RGB;

   g_globals.debugFlags = 0;

   InitButtonMap();
   InitBattleStartRects();
   Game_Init( &( g_globals.game ) );
   g_globals.shutdown = False;
   g_globals.isAnimatingBattleStart = False;
   g_globals.isAnimatingBattleAttack = False;

   while ( 1 )
   {
      Clock_StartFrame( &( g_globals.game.clock ) );
      Input_ResetState( &( g_globals.game.input ) );
      
      while ( PeekMessageA( &msg, g_globals.hWndMain, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageA( &msg );
      }

      if ( g_globals.isAnimatingBattleStart )
      {
         BattleStartAnimationTic();
      }
      else if ( g_globals.isAnimatingBattleAttack )
      {
         BattleAttackAnimationTic();
      }
      else
      {
         Game_Tic( &( g_globals.game ) );
      }

      InvalidateRect( g_globals.hWndMain, 0, 0 );
      Clock_EndFrame( &( g_globals.game.clock ) );

      if ( g_globals.shutdown )
      {
         break;
      }
   }

   WinPixelBuffer_CleanUp( &( g_globals.screenBuffer ) );
   return 0;
}

void Serial_PrintLn( const char* msg )
{
   OutputDebugStringA( msg );
}

void Battle_WinAnimateStart()
{
   g_globals.isAnimatingBattleStart = True;
   g_globals.battleStartAnimationFrame = 0;
   g_globals.animationSecondsElapsed = 0.0f;
}

void Battle_WinAnimateAttack()
{
   g_globals.isAnimatingBattleAttack = True;
   g_globals.hasPaddedBattleAttackStart = False;
   g_globals.hasBattleAttackFlashed = False;
   g_globals.hasPaddedBattleAttackEnd = False;
   g_globals.battleAttackFlashCounter = 0;
   g_globals.animationSecondsElapsed = 0.0f;
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
      case WM_PAINT:
         RenderScreen();
         break;
      default:
         result = DefWindowProcA( hWnd, uMsg, wParam, lParam );
   }

   return result;
}

internal void InitButtonMap()
{
   g_globals.buttonMap[BUTTON_LEFT] = VK_LEFT;
   g_globals.buttonMap[BUTTON_UP] = VK_UP;
   g_globals.buttonMap[BUTTON_RIGHT] = VK_RIGHT;
   g_globals.buttonMap[BUTTON_DOWN] = VK_DOWN;
   g_globals.buttonMap[BUTTON_A] = 0x41; // A
   g_globals.buttonMap[BUTTON_B] = VK_ESCAPE;
}

internal void InitBattleStartRects()
{
   g_globals.battleStartRects[0] = (Vector4u16_t) { 192, 80, 16, 16 };
   g_globals.battleStartRects[1] = (Vector4u16_t) { 192, 96, 16, 16 };
   g_globals.battleStartRects[2] = (Vector4u16_t) { 176, 96, 16, 16 };
   g_globals.battleStartRects[3] = (Vector4u16_t) { 176, 80, 16, 16 };
   g_globals.battleStartRects[4] = (Vector4u16_t) { 176, 64, 16, 16 };
   g_globals.battleStartRects[5] = (Vector4u16_t) { 192, 64, 16, 16 };
   g_globals.battleStartRects[6] = (Vector4u16_t) { 208, 64, 16, 16 };
   g_globals.battleStartRects[7] = (Vector4u16_t) { 208, 80, 16, 16 };
   g_globals.battleStartRects[8] = (Vector4u16_t) { 208, 96, 16, 16 };
   g_globals.battleStartRects[9] = (Vector4u16_t) { 208, 112, 16, 16 };
   g_globals.battleStartRects[10] = (Vector4u16_t) { 192, 112, 16, 16 };
   g_globals.battleStartRects[11] = (Vector4u16_t) { 176, 112, 16, 16 };
   g_globals.battleStartRects[12] = (Vector4u16_t) { 160, 112, 16, 16 };
   g_globals.battleStartRects[13] = (Vector4u16_t) { 160, 96, 16, 16 };
   g_globals.battleStartRects[14] = (Vector4u16_t) { 160, 80, 16, 16 };
   g_globals.battleStartRects[15] = (Vector4u16_t) { 160, 64, 16, 16 };
   g_globals.battleStartRects[16] = (Vector4u16_t) { 160, 48, 16, 16 };
   g_globals.battleStartRects[17] = (Vector4u16_t) { 176, 48, 16, 16 };
   g_globals.battleStartRects[18] = (Vector4u16_t) { 192, 48, 16, 16 };
   g_globals.battleStartRects[19] = (Vector4u16_t) { 208, 48, 16, 16 };
   g_globals.battleStartRects[20] = (Vector4u16_t) { 224, 48, 16, 16 };
   g_globals.battleStartRects[21] = (Vector4u16_t) { 224, 64, 16, 16 };
   g_globals.battleStartRects[22] = (Vector4u16_t) { 224, 80, 16, 16 };
   g_globals.battleStartRects[23] = (Vector4u16_t) { 224, 96, 16, 16 };
   g_globals.battleStartRects[24] = (Vector4u16_t) { 224, 112, 16, 16 };
   g_globals.battleStartRects[25] = (Vector4u16_t) { 224, 128, 16, 16 };
   g_globals.battleStartRects[26] = (Vector4u16_t) { 208, 128, 16, 16 };
   g_globals.battleStartRects[27] = (Vector4u16_t) { 192, 128, 16, 16 };
   g_globals.battleStartRects[28] = (Vector4u16_t) { 176, 128, 16, 16 };
   g_globals.battleStartRects[29] = (Vector4u16_t) { 160, 128, 16, 16 };
   g_globals.battleStartRects[30] = (Vector4u16_t) { 144, 128, 16, 16 };
   g_globals.battleStartRects[31] = (Vector4u16_t) { 144, 112, 16, 16 };
   g_globals.battleStartRects[32] = (Vector4u16_t) { 144, 96, 16, 16 };
   g_globals.battleStartRects[33] = (Vector4u16_t) { 144, 80, 16, 16 };
   g_globals.battleStartRects[34] = (Vector4u16_t) { 144, 64, 16, 16 };
   g_globals.battleStartRects[35] = (Vector4u16_t) { 144, 48, 16, 16 };
   g_globals.battleStartRects[36] = (Vector4u16_t) { 144, 32, 16, 16 };
   g_globals.battleStartRects[37] = (Vector4u16_t) { 160, 32, 16, 16 };
   g_globals.battleStartRects[38] = (Vector4u16_t) { 176, 32, 16, 16 };
   g_globals.battleStartRects[39] = (Vector4u16_t) { 192, 32, 16, 16 };
   g_globals.battleStartRects[40] = (Vector4u16_t) { 208, 32, 16, 16 };
   g_globals.battleStartRects[41] = (Vector4u16_t) { 224, 32, 16, 16 };
   g_globals.battleStartRects[42] = (Vector4u16_t) { 240, 32, 16, 16 };
   g_globals.battleStartRects[43] = (Vector4u16_t) { 240, 48, 16, 16 };
   g_globals.battleStartRects[44] = (Vector4u16_t) { 240, 64, 16, 16 };
   g_globals.battleStartRects[45] = (Vector4u16_t) { 240, 80, 16, 16 };
   g_globals.battleStartRects[46] = (Vector4u16_t) { 240, 96, 16, 16 };
   g_globals.battleStartRects[47] = (Vector4u16_t) { 240, 112, 16, 16 };
   g_globals.battleStartRects[48] = (Vector4u16_t) { 240, 128, 16, 16 };
}

internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags )
{
   Bool_t keyWasDown = ( flags & ( (LONG_PTR)1 << 30 ) ) != 0 ? True : False;
   Bool_t keyIsDown = ( flags & ( (LONG_PTR)1 << 31 ) ) == 0 ? True : False;
   uint8_t i;

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

         for ( i = 0; i < BUTTON_COUNT; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonPressed( &( g_globals.game.input ), i );
               break;
            }
         }

         switch ( keyCode )
         {
            case VK_DEBUG_TILEPASSABILITY:
               g_globals.debugFlags = ( g_globals.debugFlags & DEBUGMASK_TILEPASSABILITY ) ? 0 : DEBUGMASK_TILEPASSABILITY;
               Game_RefreshMap( &( g_globals.game ) );
               break;
            case VK_DEBUG_ENCOUNTERRATE:
               g_globals.debugFlags = ( g_globals.debugFlags & DEBUGMASK_ENCOUNTERRATE ) ? 0 : DEBUGMASK_ENCOUNTERRATE;
               Game_RefreshMap( &( g_globals.game ) );
               break;
         }
      }
      else
      {
         for ( i = 0; i < BUTTON_COUNT; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonReleased( &( g_globals.game.input ), i );
               break;
            }
         }
      }
   }
}

internal void RenderScreen()
{
   PAINTSTRUCT pc;
   HDC dc = BeginPaint( g_globals.hWndMain, &pc );

   StretchDIBits(
      dc,
      0, 0, (int)( SCREEN_WIDTH * GRAPHICS_SCALE ), (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ), // dest
      0, 0, g_globals.screenBuffer.w, g_globals.screenBuffer.h, // src
      g_globals.screenBuffer.memory,
      &( g_globals.bmpInfo ),
      DIB_RGB_COLORS, SRCCOPY
   );

   EndPaint( g_globals.hWndMain, &pc );
}

internal void BattleStartAnimationTic()
{
   Vector4u16_t rect;
   float animationFrameSeconds = 0.01f;

   g_globals.animationSecondsElapsed += FRAME_SECONDS;

   while ( g_globals.animationSecondsElapsed > animationFrameSeconds )
   {
      g_globals.animationSecondsElapsed -= animationFrameSeconds;
      rect = g_globals.battleStartRects[g_globals.battleStartAnimationFrame];
      Screen_DrawRect( &( g_globals.game.screen ), rect.x, rect.y, rect.w, rect.h, BLACK );
      g_globals.battleStartAnimationFrame++;

      if ( g_globals.battleStartAnimationFrame >= 49 )
      {
         g_globals.isAnimatingBattleStart = False;
         Screen_DrawEnemy( &( g_globals.game ), 160, 40 );
         Battle_StartHUD( &( g_globals.game ) );
         break;
      }
   }
}

internal void BattleAttackAnimationTic()
{
   uint32_t totalFlashes = 6;
   float flashSeconds = 0.05f;
   float paddingSeconds = 0.4f;

   g_globals.animationSecondsElapsed += FRAME_SECONDS;

   if ( !g_globals.hasPaddedBattleAttackStart && g_globals.animationSecondsElapsed > paddingSeconds )
   {
      g_globals.animationSecondsElapsed -= paddingSeconds;
      g_globals.hasPaddedBattleAttackStart = True;
   }

   if ( g_globals.hasPaddedBattleAttackStart && !g_globals.hasBattleAttackFlashed )
   {
      while ( g_globals.animationSecondsElapsed > flashSeconds )
      {
         g_globals.animationSecondsElapsed -= flashSeconds;

         if ( g_globals.battleAttackFlashCounter % 2 == 0 )
         {
            Screen_WipeEnemy( &( g_globals.game ), 160, 40 );
         }
         else
         {
            Screen_DrawEnemy( &( g_globals.game ), 160, 40 );
         }

         g_globals.battleAttackFlashCounter++;

         if ( g_globals.battleAttackFlashCounter == totalFlashes )
         {
            g_globals.hasBattleAttackFlashed = True;
            break;
         }
      }
   }

   if ( g_globals.hasPaddedBattleAttackStart &&
        g_globals.hasBattleAttackFlashed &&
        !g_globals.hasPaddedBattleAttackEnd &&
        g_globals.animationSecondsElapsed > paddingSeconds )
   {
      g_globals.isAnimatingBattleAttack = False;
      Battle_ExecuteAttack( &( g_globals.game ) );
   }
}
