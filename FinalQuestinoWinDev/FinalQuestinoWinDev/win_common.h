#if !defined( WIN_COMMON_H )
#define WIN_COMMON_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <gl/GL.h>

#include "enums.h"
#include "game.h"
#include "win_pixel_buffer.h"

// not really necessary, but makes things easier to distinguish
#define internal static
#define global static
#define local_persist static

#define STRING_SIZE_DEFAULT   1024
#define GRAPHICS_SCALE        2.0f

typedef struct GlobalObjects_t
{
   HWND hWndMain;
   LARGE_INTEGER performanceFrequency;
   uint32_t buttonMap[(int)Button_Count];
   Game_t game;
   Bool_t shutdown;
   WinPixelBuffer_t screenBuffer;
   GLuint screenBufferTexture;
}
GlobalObjects_t;

global GlobalObjects_t g_globals;

#endif
