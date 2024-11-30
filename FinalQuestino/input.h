#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"
#include "button_state.h"

#if !defined VISUAL_STUDIO_DEV
#define PIN_ANALOG_X    A9
#define PIN_ANALOG_Y    A8
#define PIN_A_BUTTON    12
#define PIN_B_BUTTON    13
#endif

typedef struct Game_t Game_t;

typedef struct Input_t
{
   ButtonState_t buttonStates[BUTTON_COUNT];
}
Input_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Input_Init( Input_t* input );
void Input_Read( Input_t* input );
void Input_Handle( Game_t* game );

#if defined( VISUAL_STUDIO_DEV )
void Input_ResetState( Input_t* input );
void Input_ButtonPressed( Input_t* input, uint8_t button );
void Input_ButtonReleased( Input_t* input, uint8_t button );
#endif

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
