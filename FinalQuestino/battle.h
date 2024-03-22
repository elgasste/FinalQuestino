#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

typedef struct cGame_t cGame_t;

typedef struct cBattle_t
{
   cEnemy_t enemy;
}
cBattle_t;

#if defined( __cplusplus )
extern "C" {
#endif 

void cBattle_Start( cGame_t* game );
void cBattle_Done( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
