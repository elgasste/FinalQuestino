#include "win_pixel_buffer.h"
#include "win_common.h"

void WinPixelBuffer_Init( WinPixelBuffer_t* buffer, uint32_t w, uint32_t h )
{
   buffer->w = w;
   buffer->h = h;
   buffer->memory = (uint32_t*)malloc( sizeof( uint32_t ) * w * h );
}

void WinPixelBuffer_CleanUp( WinPixelBuffer_t* buffer )
{
   free( buffer->memory );
   buffer->memory = 0;
   buffer->w = 0;
   buffer->h = 0;
}
