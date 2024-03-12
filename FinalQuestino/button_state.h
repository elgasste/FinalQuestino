#if !defined( BUTTON_STATE_H )
#define BUTTON_STATE_H

#include "common.h"

typedef struct cButtonState_t
{
   cBool_t pressed;
   cBool_t released;
   cBool_t down;
}
cButtonState_t;

#endif // BUTTON_STATE_H
