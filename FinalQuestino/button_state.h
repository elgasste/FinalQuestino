#if !defined( BUTTON_STATE_H )
#define BUTTON_STATE_H

#include "common.h"

typedef struct ButtonState_t
{
   Bool_t pressed;
   Bool_t released;
   Bool_t down;
}
ButtonState_t;

#endif // BUTTON_STATE_H
