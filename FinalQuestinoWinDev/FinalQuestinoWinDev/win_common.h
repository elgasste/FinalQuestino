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

#define STRING_SIZE_DEFAULT         1024
#define GRAPHICS_SCALE              2.0f

#define VK_DEBUG_TILEPASSABILITY    0x31     // 1
#define VK_DEBUG_ENCOUNTERRATE      0x32     // 2

#define DEBUGMASK_TILEPASSABILITY   0x1
#define DEBUGMASK_ENCOUNTERRATE     0x2

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
   uint32_t battleStartAnimationFrame;
   Vector4u16_t battleStartRects[49];

   Bool_t isAnimatingBattleAttack;
   uint32_t battleAttackFlashCounter;
   Bool_t hasPaddedBattleAttackStart;
   Bool_t hasBattleAttackFlashed;
   Bool_t hasPaddedBattleAttackEnd;

   float animationSecondsElapsed;

   uint32_t debugFlags;
}
GlobalObjects_t;

GlobalObjects_t g_globals;

#endif // WIN_COMMON_H
