#if !defined( INPUT_READER_H )
#define INPUT_READER_H

#include "common.h"
#include "button_state.h"
#include "enums.h"

#define PIN_ANALOG_X A15
#define PIN_ANALOG_Y A14

typedef struct cInputReader_t
{
  cButtonState_t buttonStates[cButton_Count];
}
cInputReader_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cInputReader_Init( cInputReader_t* inputReader );
void cInputReader_ReadInput( cInputReader_t* inputReader );

#if defined( __cplusplus )
}
#endif

#endif // INPUT_READER_H
