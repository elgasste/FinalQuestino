#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"
#include "button_state.h"
#include "enums.h"

#define PIN_ANALOG_X A15
#define PIN_ANALOG_Y A14

typedef struct cGame_t cGame_t;

typedef struct cInput_t
{
   cButtonState_t buttonStates[cButton_Count];
}
cInput_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cInput_Init( cInput_t* input );
void cInput_Read( cInput_t* input );
void cInput_Handle( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
