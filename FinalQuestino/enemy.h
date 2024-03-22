#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

typedef struct cEnemy_t
{
   char name[16];
   cBattleStats_t stats;
}
cEnemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cEnemy_Generate( cEnemy_t* enemy );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
