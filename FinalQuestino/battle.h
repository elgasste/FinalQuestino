#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"

typedef struct cGame_t cGame_t;

#if defined( __cplusplus )
extern "C" {
#endif 

void cBattle_Start( cGame_t* game );
void cBattle_Done( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
