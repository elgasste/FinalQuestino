#include "input_reader.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

static void cInputReader_UpdateButtonState( cButtonState_t* buttonState, cBool_t down );

void cInputReader_Init( cInputReader_t* inputReader )
{
  uint8_t i;

  for( i = 0; i < (uint8_t)cButton_Count; i++ )
  {
    inputReader->buttonStates[i].pressed = cFalse;
    inputReader->buttonStates[i].released = cFalse;
    inputReader->buttonStates[i].down = cFalse;
  }
}

void cInputReader_ReadInput( cInputReader_t* inputReader )
{
  int xValue = analogRead( PIN_ANALOG_X );
  int yValue = analogRead( PIN_ANALOG_Y );

  cInputReader_UpdateButtonState( &( inputReader->buttonStates[cButton_Left ] ), xValue > ANALOG_THRESHOLD_HIGH );
  cInputReader_UpdateButtonState( &( inputReader->buttonStates[cButton_Up ] ), yValue < ANALOG_THRESHOLD_LOW );
  cInputReader_UpdateButtonState( &( inputReader->buttonStates[cButton_Right ] ), xValue < ANALOG_THRESHOLD_LOW );
  cInputReader_UpdateButtonState( &( inputReader->buttonStates[cButton_Down ] ), yValue > ANALOG_THRESHOLD_HIGH );
}

static void cInputReader_UpdateButtonState( cButtonState_t* buttonState, cBool_t down )
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
