#include "input_reader.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

static void cInput_UpdateButtonState( cButtonState_t* buttonState, cBool_t down );

void cInput_Init( cInput_t* input )
{
  uint8_t i;

  for( i = 0; i < (uint8_t)cButton_Count; i++ )
  {
    input->buttonStates[i].pressed = cFalse;
    input->buttonStates[i].released = cFalse;
    input->buttonStates[i].down = cFalse;
  }
}

void cInput_Read( cInput_t* input )
{
  int xValue = analogRead( PIN_ANALOG_X );
  int yValue = analogRead( PIN_ANALOG_Y );

  cInput_UpdateButtonState( &( input->buttonStates[cButton_Left ] ), xValue > ANALOG_THRESHOLD_HIGH );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Up ] ), yValue < ANALOG_THRESHOLD_LOW );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Right ] ), xValue < ANALOG_THRESHOLD_LOW );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Down ] ), yValue > ANALOG_THRESHOLD_HIGH );
}

static void cInput_UpdateButtonState( cButtonState_t* buttonState, cBool_t down )
{
  if ( down )
  {
    buttonState->released = cFalse;
    buttonState->pressed = buttonState->down ? cFalse : cTrue;
  }
  else
  {
    buttonState->pressed = cFalse;
    buttonState->released = buttonState->down ? cTrue : cFalse;
  }

  buttonState->down = down;
}
