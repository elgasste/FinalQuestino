#if !defined( INPUT_READER_H )
#define INPUT_READER_H

#include "common.h"
#include "button_state.h"
#include "enums.h"

#define PIN_ANALOG_X A15
#define PIN_ANALOG_Y A14

// MUFFINS: rename this, it's not an input reader. Also rename this whole file to just input.h,
// and put the handler functions in.
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

#if defined( __cplusplus )
}
#endif

#endif // INPUT_READER_H
