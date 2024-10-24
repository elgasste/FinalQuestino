#include "win_blit.h"
#include "win_common.h"

internal void Blit_PrepareTextureForDrawing( GLuint textureHandle, WinPixelBuffer_t* buffer,
                                             float screenX, float screenY,
                                             float width, float height,
                                             float scale, uint32_t color );

void Blit_Texture( unsigned int textureHandle, WinPixelBuffer_t* buffer, float screenX, float screenY, float scale )
{
   Blit_PrepareTextureForDrawing( textureHandle, buffer, screenX, screenY, buffer->w, buffer->h, scale, 0xFFFFFFFF );

   glBegin( GL_QUADS );

   glTexCoord2f( 0.0f, 0.0f );
   glVertex2f( -1.0f, -1.0f );
   glTexCoord2f( 1.0f, 0.0f );
   glVertex2f( 1.0f, -1.0f );
   glTexCoord2f( 1.0f, 1.0f );
   glVertex2f( 1.0f, 1.0f );
   glTexCoord2f( 0.0f, 1.0f );
   glVertex2f( -1.0f, 1.0f );

   glEnd();
}

internal void Blit_PrepareTextureForDrawing( GLuint textureHandle, WinPixelBuffer_t* buffer,
                                             float screenX, float screenY,
                                             float width, float height,
                                             float scale, uint32_t color )
{
   GLint r = ( color >> 16 ) & 0xFF;
   GLint g = ( color >> 8 ) & 0xFF;
   GLint b = color & 0xFF;
   GLint a = ( color >> 24 ) & 0xFF;

   glViewport( (GLint)screenX, (GLint)screenY, (GLsizei)( width * scale ), (GLsizei)( height * scale ) );

   glBindTexture( GL_TEXTURE_2D, textureHandle );
   glTexImage2D( GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 buffer->w,
                 buffer->h,
                 0,
                 GL_BGRA_EXT,
                 GL_UNSIGNED_BYTE,
                 (GLvoid*)( buffer->memory ) );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

   glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   glEnable( GL_TEXTURE_2D );

   glColor4f( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnable( GL_BLEND );

   glMatrixMode( GL_TEXTURE );
   glLoadIdentity();

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
}