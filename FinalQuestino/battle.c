#include "game.h"
#include "random.h"

internal void Battle_AnimateStart( Game_t* game );

void Battle_Start( Game_t* game )
{
   uint8_t enemyIndex;
   uint8_t playerTileX, playerTileY;
   Vector4u8_t* specialRegion = &( game->tileMap.enemySpecialRegion );

   if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) )
   {
      Enemy_Load( &( game->battle.enemy ), SPECIALENEMYID_GREENDRAGON );
   }
   else if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) )
   {
      Enemy_Load( &( game->battle.enemy ), SPECIALENEMYID_GOLEM );
   }
   else if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT ) )
   {
      Enemy_Load( &( game->battle.enemy ), SPECIALENEMYID_AXEKNIGHT );
   }
   else
   {
      enemyIndex = Random_Uint8( 0, MAP_TILE_ENEMY_INDEX_COUNT - 1 );
      playerTileX = ( uint8_t )( ( game->player.position.x + ( PLAYER_HITBOX_SIZE / 2 ) ) / MAP_TILE_SIZE );
      playerTileY = ( uint8_t )( ( game->player.position.y + ( PLAYER_HITBOX_SIZE / 2 ) ) / MAP_TILE_SIZE );

      if ( playerTileX >= specialRegion->x && playerTileX <= ( specialRegion->x + specialRegion->w ) &&
           playerTileY >= specialRegion->y && playerTileY <= ( specialRegion->y + specialRegion->h ) )
      {
         Enemy_Load( &( game->battle.enemy ), game->tileMap.enemySpecialIndexes[enemyIndex] );
      }
      else
      {
         Enemy_Load( &( game->battle.enemy ), game->tileMap.enemyIndexes[enemyIndex] );
      }
   }

   Battle_AnimateStart( game );
}

void Battle_StartHUD( Game_t* game )
{
   char str[32];

   // quick stats
   Screen_DrawRect( &( game->screen ), 16, 16, 76, 36, DARKGRAY );
   snprintf( str, 32, "HP:%u", game->player.stats.HitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   snprintf( str, 32, "MP:%u", game->player.stats.MagicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );

   if ( game->battle.enemy.indefiniteArticle == IndefiniteArticle_A )
   {
      snprintf( str, 32, "A %s approaches!", game->battle.enemy.name );
   }
   else
   {
      snprintf( str, 32, "An %s approaches!", game->battle.enemy.name );
   }

   Game_ShowMessage( game, str );
}

void Battle_Done( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 36 );
   Screen_WipeTileMapSection( game, 128, 32, 112, 112 );
   Game_WipeMessage( game );
}

internal void Battle_AnimateStart( Game_t* game )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( game );
   Battle_WinAnimateStart();
#else
   Screen_DrawRect( &( game->screen ), 176, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 128, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 128, 16, 16, BLACK ); DELAY_MS( 10 );

   Screen_DrawEnemy( game, 144, 40 );
   Battle_StartHUD( game );
#endif
}
