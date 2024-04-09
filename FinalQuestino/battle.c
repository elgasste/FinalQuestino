#include "game.h"
#include "random.h"

static cBattle_AnimateStart( cGame_t* game );

void cBattle_Start( cGame_t* game )
{
   uint8_t enemyIndex;
   uint8_t playerTileX, playerTileY;
   cVector4u8_t* specialRegion = &( game->tileMap.enemySpecialRegion );
   char str[32];

   if ( cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) )
   {
      cEnemy_Load( &( game->battle.enemy ), SPECIALENEMYID_GREENDRAGON );
   }
   else if ( cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) )
   {
      cEnemy_Load( &( game->battle.enemy ), SPECIALENEMYID_GOLEM );
   }
   else if ( cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT ) )
   {
      cEnemy_Load( &( game->battle.enemy ), SPECIALENEMYID_AXEKNIGHT );
   }
   else
   {
      enemyIndex = cRandom_Uint8( 0, MAP_TILE_ENEMY_INDEX_COUNT - 1 );
      playerTileX = ( game->player.position.x + ( PLAYER_HITBOX_SIZE / 2 ) ) / MAP_TILE_SIZE;
      playerTileY = ( game->player.position.y + ( PLAYER_HITBOX_SIZE / 2 ) ) / MAP_TILE_SIZE;

      if ( playerTileX >= specialRegion->x && playerTileX <= ( specialRegion->x + specialRegion->w ) &&
           playerTileY >= specialRegion->y && playerTileY <= ( specialRegion->y + specialRegion->h ) )
      {
         cEnemy_Load( &( game->battle.enemy ), game->tileMap.enemySpecialIndexes[enemyIndex] );
      }
      else
      {
         cEnemy_Load( &( game->battle.enemy ), game->tileMap.enemyIndexes[enemyIndex] );
      }
   }

   cBattle_AnimateStart( game );

   // quick stats
   cScreen_DrawRect( &( game->screen ), 16, 16, 76, 36, BLACK );
   snprintf( str, 32, "HP:%u", game->player.stats.HitPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
   snprintf( str, 32, "MP:%u", game->player.stats.MagicPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );

   snprintf( str, 32, "A %s approaches!", game->battle.enemy.name );
   cGame_ShowMessage( game, str );

   cScreen_DrawEnemy( game, 144, 40 );
}

void cBattle_Done( cGame_t* game )
{
   cScreen_WipeTileMapSection( game, 16, 16, 76, 36 );
   cScreen_WipeTileMapSection( game, 128, 32, 112, 112 );
   cGame_WipeMessage( game );
}

static cBattle_AnimateStart( cGame_t* game )
{
   cScreen_DrawRect( &( game->screen ), 176, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 128, 16, 16, BLACK ); delay( 10 );
}
