#if !defined( ENUMS_H )
#define ENUMS_H

#include "common.h"

typedef enum Direction_t
{
   Direction_Left = 0,
   Direction_Up,
   Direction_Right,
   Direction_Down
}
Direction_t;

typedef enum GameState_t
{
   GameState_Init = 0,
   GameState_Map,
   GameState_MapMenu,
   GameState_MapMessage,
   GameState_MapStatus,
   GameState_Battle
}
GameState_t;

typedef enum Button_t
{
   Button_Left = 0,
   Button_Up,
   Button_Right,
   Button_Down,

   Button_A,
   Button_B,

   Button_Count
}
Button_t;

typedef enum MenuIndex_t
{
   MenuIndex_Map = 0
}
MenuIndex_t;

typedef enum IndefiniteArticle_t
{
   IndefiniteArticle_A = 0,
   IndefiniteArticle_An
}
IndefiniteArticle_t;

#endif // ENUMS_H
