#include "game.h"
#include "random.h"
#include "menu.h"

internal void Battle_AnimateStart( Game_t* game );
internal void Battle_ShowMessage( Game_t* game, const char* message );
internal void Battle_WipeMessage( Game_t* game );

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

   Screen_WipePlayer( game );
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

   Battle_ShowMessage( game, str );
   Menu_Load( &( game->menu ), MenuIndex_BattleMain );
   Menu_Draw( game );
   game->state = GameState_BattleMenuMain;
}

void Battle_Attack( Game_t* game )
{
   Menu_Wipe( game );
   Battle_ShowMessage( game, "Sliced him up real good, you win!" );
   game->state = GameState_BattleResult;
}

void Battle_Spell( Game_t* game )
{
   Menu_Wipe( game );
   Battle_ShowMessage( game, "Abra cadabra, you win!" );
   game->state = GameState_BattleResult;
}

void Battle_Item( Game_t* game )
{
   Menu_Wipe( game );
   Battle_ShowMessage( game, "Your spare change hit him in the eye, you win!" );
   game->state = GameState_BattleResult;
}

void Battle_Flee( Game_t* game )
{
   Menu_Wipe( game );
   Battle_ShowMessage( game, "You dodged and weaved your way out, nice work!" );
   game->state = GameState_BattleResult;
}

void Battle_Done( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 36 );       // quick stats
   Screen_WipeTileMapSection( game, 144, 32, 112, 112 );    // enemy
   Menu_Wipe( game );
   Battle_WipeMessage( game );
   Screen_DrawActors( game );
   game->state = GameState_Map;
}

internal void Battle_AnimateStart( Game_t* game )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( game );
   Battle_WinAnimateStart();
#else
   Screen_DrawRect( &( game->screen ), 192, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 128, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 144, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 160, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 176, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 192, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 208, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 224, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 32, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 48, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 64, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 80, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 96, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 112, 16, 16, BLACK ); DELAY_MS( 10 );
   Screen_DrawRect( &( game->screen ), 240, 128, 16, 16, BLACK ); DELAY_MS( 10 );

   Screen_DrawEnemy( game, 160, 40 );
   Battle_StartHUD( game );
#endif
}

internal void Battle_ShowMessage( Game_t* game, const char* message )
{
   Screen_DrawRect( &( game->screen ), 96, 152, 208, 72, DARKGRAY );
   Screen_DrawWrappedText( &( game->screen ), message, 104, 160, 24, 8, DARKGRAY, WHITE );
}

internal void Battle_WipeMessage( Game_t* game )
{
   Screen_WipeTileMapSection( game, 96, 152, 208, 72 );
}
