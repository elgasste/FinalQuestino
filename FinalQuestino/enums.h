#if !defined( ENUMS_H )
#define ENUMS_H

#include "common.h"

typedef enum cDirection_t
{
   cDirection_Left = 0,
   cDirection_Up,
   cDirection_Right,
   cDirection_Down
}
cDirection_t;

typedef enum cGameState_t
{
   cGameState_Init = 0,
   cGameState_Map,
   cGameState_MapMenu
}
cGameState_t;

typedef enum cButton_t
{
   cButton_Left = 0,
   cButton_Up,
   cButton_Right,
   cButton_Down,

   cButton_A,
   cButton_B,

   cButton_Count
}
cButton_t;

#endif // ENUMS_H
