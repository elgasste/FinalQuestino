#include "game.h"
#include "random.h"
#include "menu.h"

internal void Battle_AnimateStart( Game_t* game );
internal void Battle_ShowMessage( Game_t* game, const char* message );
internal void Battle_WipeMessage( Game_t* game );
internal void Battle_AnimateAttack( Game_t* game );

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
   snprintf( str, 32, PSTR( "HP:%u" ), game->player.stats.HitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   snprintf( str, 32, PSTR( "MP:%u" ), game->player.stats.MagicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );

   if ( game->battle.enemy.indefiniteArticle == INDEFINITEARTICLE_A )
   {
      snprintf( str, 32, PSTR( "A %s approaches! Command?" ), game->battle.enemy.name );
   }
   else
   {
      snprintf( str, 32, PSTR( "An %s approaches! Command?" ), game->battle.enemy.name );
   }

   Battle_ShowMessage( game, str );
   Menu_Load( &( game->menu ), MENUINDEX_BATTLEMAIN );
   Menu_Draw( game );
   game->state = GAMESTATE_BATTLEMENUMAIN;
}

void Battle_Attack( Game_t* game )
{
   Menu_WipeCarat( game );
   Battle_ShowMessage( game, PSTR( "You Attack!" ) );
   game->state = GAMESTATE_BATTLEATTACKANIMATION;
   Battle_AnimateAttack( game );
}

void Battle_Spell( Game_t* game )
{
   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   Battle_ShowMessage( game, PSTR( "You yell 'ABRA CADABRA!!', which is a death spell. You win!" ) );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Item( Game_t* game )
{
   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   Battle_ShowMessage( game, PSTR( "Your spare change hit him in the eye and killed him, you win!" ) );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Flee( Game_t* game )
{
   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   Battle_ShowMessage( game, PSTR( "You dodged and weaved your way out, nice work!" ) );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Done( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 36 );       // quick stats
   Screen_WipeTileMapSection( game, 144, 32, 112, 112 );    // enemy
   Menu_Wipe( game );
   Battle_WipeMessage( game );
   Screen_DrawActors( game );
   game->state = GAMESTATE_MAP;
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

internal void Battle_AnimateAttack( Game_t* game )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( game );
   Battle_WinAnimateAttack();
#else
   DELAY_MS( 400 );
   Screen_WipeEnemy( game, 160, 40 ); DELAY_MS( 80 );
   Screen_DrawEnemy( game, 160, 40 ); DELAY_MS( 80 );
   Screen_WipeEnemy( game, 160, 40 ); DELAY_MS( 80 );
   Screen_DrawEnemy( game, 160, 40 ); DELAY_MS( 80 );
   Screen_WipeEnemy( game, 160, 40 ); DELAY_MS( 80 );
   Screen_DrawEnemy( game, 160, 40 );
   DELAY_MS( 400 );

   Battle_ExecuteAttack( game );
#endif
}

void Battle_ExecuteAttack( Game_t* game )
{
   // TODO: actually cause damage and check if the enemy has been killed
   Battle_ShowMessage( game, PSTR( "You caused damage, probably! Command?" ) );
   game->state = GAMESTATE_BATTLEMENUMAIN;
}
