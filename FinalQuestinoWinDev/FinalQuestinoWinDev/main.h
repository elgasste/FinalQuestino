#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

// MUFFINS: take these out if we don't need them
#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
//#include <gl/GL.h>
//#include <Shlwapi.h>
//#include <time.h>

#include "common.h"

// not really necessary, but makes things easier to distinguish
#define internal static
#define global static
#define local_persist static

#define STRING_SIZE_DEFAULT 1024