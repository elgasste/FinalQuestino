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

typedef struct Vector4u16_t
{
   uint16_t x;
   uint16_t y;
   uint16_t w;
   uint16_t h;
}
Vector4u16_t;

#endif // VECTOR_H
