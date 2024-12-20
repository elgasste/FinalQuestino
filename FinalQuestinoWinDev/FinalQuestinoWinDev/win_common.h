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
#define GRAPHICS_SCALE              3.0f

#define VK_DEBUG_PASSABLETILES      0x31     // 1
#define VK_DEBUG_ENCOUNTERRATES     0x32     // 2
#define VK_DEBUG_FASTWALK           0x33     // 3
#define VK_DEBUG_NOENCOUNTERS       0x34     // 4
#define VK_DEBUG_NOCLIP             0x35     // 5
#define VK_DEBUG_GETKEY             0x36     // 6
#define VK_DEBUG_GETGOLD            0x37     // 7
#define VK_DEBUG_CLEARFLAGS         0x30     // 0

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

   Bool_t isAnimatingBattleFlee;

   float animationSecondsElapsed;
}
GlobalObjects_t;

GlobalObjects_t g_globals;

#endif // WIN_COMMON_H
