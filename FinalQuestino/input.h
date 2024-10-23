#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"
#include "button_state.h"
#include "enums.h"

#define PIN_ANALOG_X    A15
#define PIN_ANALOG_Y    A14
#define PIN_A_BUTTON    50
#define PIN_B_BUTTON    52

typedef struct Game_t Game_t;

typedef struct Input_t
{
   ButtonState_t buttonStates[Button_Count];
}
Input_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Input_Init( Input_t* input );
void Input_Read( Input_t* input );
void Input_Handle( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
