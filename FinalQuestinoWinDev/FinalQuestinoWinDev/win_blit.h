#if !defined( WIN_BLIT_H )
#define WIN_BLIT_H

#include "common.h"

typedef struct WinPixelBuffer_t WinPixelBuffer_t;

void Blit_Texture( unsigned int textureHandle, WinPixelBuffer_t* buffer, float screenX, float screenY, float scale );

#endif
