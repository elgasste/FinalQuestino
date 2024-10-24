#if !defined( MENU_H )
#define MENU_H

#include "common.h"
#include "enums.h"

typedef struct Game_t Game_t;

typedef struct Menu_t
{
   MenuIndex_t index;
   uint8_t optionIndex;
   uint8_t optionCount;
   float caratSeconds;
   Bool_t showCarat;
}
Menu_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Menu_Load( Menu_t* menu, MenuIndex_t index );
void Menu_Draw( Game_t* game );
void Menu_Wipe( Game_t* game );
void Menu_Tic( Game_t* game );
void Menu_ScrollDown( Game_t* game );
void Menu_ScrollUp( Game_t* game );
void Menu_Select( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // MENU_H
