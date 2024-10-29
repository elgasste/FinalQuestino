#if !defined( WIN_COMMON_H )
#define WIN_COMMON_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "game.h"
#include "win_pixel_buffer.h"
#include "vector.h"

#define STRING_SIZE_DEFAULT   1024
#define GRAPHICS_SCALE        2.0f

typedef struct GlobalObjects_t
{
   HWND hWndMain;
   LARGE_INTEGER performanceFrequency;
   uint32_t buttonMap[BUTTON_COUNT];
   Game_t game;
   Bool_t shutdown;
   WinPixelBuffer_t screenBuffer;
   BITMAPINFO bmpInfo;

   Bool_t isAnimatingBattleStart;
   float animationSecondsElapsed;
   uint32_t animationFrame;
   Vector4u16_t battleStartRects[49];
}
GlobalObjects_t;

GlobalObjects_t g_globals;

#endif // WIN_COMMON_H
