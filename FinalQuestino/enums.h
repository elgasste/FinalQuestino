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
   cGameState_MapMenu,
   cGameState_MapMessage,
   cGameState_MapStatus,
   cGameState_Battle
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

typedef enum cMenuIndex_t
{
   cMenuIndex_Map = 0
}
cMenuIndex_t;

typedef enum cIndefiniteArticle_t
{
   cIndefiniteArticle_A = 0,
   cIndefiniteArticle_An
}
cIndefiniteArticle_t;

#endif // ENUMS_H
