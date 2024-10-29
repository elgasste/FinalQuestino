#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

typedef struct Game_t Game_t;

typedef struct Battle_t
{
   Enemy_t enemy;
}
Battle_t;

#if defined( __cplusplus )
extern "C" {
#endif 

void Battle_Start( Game_t* game );
void Battle_StartHUD( Game_t* game );
void Battle_Attack( Game_t* game );
void Battle_Spell( Game_t* game );
void Battle_Item( Game_t* game );
void Battle_Flee( Game_t* game );
void Battle_Done( Game_t* game );
void Battle_ExecuteAttack( Game_t* game );

#if defined( VISUAL_STUDIO_DEV )
void Battle_WinAnimateStart();
void Battle_WinAnimateAttack();
#endif

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
