#if !defined( VECTOR_H )
#define VECTOR_H

#include "common.h"

typedef struct Vector2f_t
{
   float x;
   float y;
}
Vector2f_t;

typedef struct Vector4u8_t
{
   uint8_t x;
   uint8_t y;
   uint8_t w;
   uint8_t h;
}
Vector4u8_t;

#endif // VECTOR_H
