#if !defined( MENU_H )
#define MENU_H

#include "common.h"
#include "enums.h"

typedef struct cGame_t cGame_t;

typedef struct cMenu_t
{
   cMenuIndex_t index;
   uint8_t optionIndex;
   uint8_t optionCount;
   float caratSeconds;
   cBool_t showCarat;
}
cMenu_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cMenu_Load( cMenu_t* menu, cMenuIndex_t index );
void cMenu_Draw( cGame_t* game );
void cMenu_Wipe( cGame_t* game );
void cMenu_Tic( cGame_t* game );
void cMenu_ScrollDown( cGame_t* game );
void cMenu_ScrollUp( cGame_t* game );
void cMenu_Select( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // MENU_H
