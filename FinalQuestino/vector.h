#if !defined( VECTOR_H )
#define VECTOR_H

#include "common.h"

typedef struct cVector2f_t
{
   float x;
   float y;
}
cVector2f_t;

typedef struct cVector4u8_t
{
   uint8_t x;
   uint8_t y;
   uint8_t w;
   uint8_t h;
}
cVector4u8_t;

#endif // VECTOR_H
