#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

typedef struct Enemy_t
{
   char name[16];
   IndefiniteArticle_t indefiniteArticle;
   BattleStats_t stats;

   uint8_t tileTextures[ENEMY_TILE_TEXTURE_COUNT][ENEMY_TILE_TEXTURE_SIZE_BYTES];
   int8_t tileTextureIndexes[ENEMY_TILE_COUNT];
   uint16_t palette[16];
}
Enemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Enemy_Load( Enemy_t* enemy, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
