#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

typedef struct cEnemy_t
{
   char name[16];
   cBattleStats_t stats;

   uint8_t tileTextures[ENEMY_TILE_TEXTURE_COUNT][ENEMY_TILE_TEXTURE_SIZE_BYTES];
   uint8_t tileTextureIndexes[ENEMY_TILE_COUNT];
}
cEnemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cEnemy_Generate( cEnemy_t* enemy, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
