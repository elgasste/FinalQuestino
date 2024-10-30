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

      // TODO: try slightly randomizing the battle stats
   }

   Screen_WipePlayer( game );
   Battle_AnimateStart( game );
}

void Battle_StartHUD( Game_t* game )
{
   char str[64];

   // quick stats
   Screen_DrawRect( &( game->screen ), 16, 16, 76, 36, DARKGRAY );
   SPRINTF_P( str, PSTR( "HP:%u" ), game->player.stats.HitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( "MP:%u" ), game->player.stats.MagicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );

   if ( game->battle.enemy.indefiniteArticle == INDEFINITEARTICLE_A )
   {
      SPRINTF_P( str, PSTR( "A %s approaches! Command?" ), game->battle.enemy.name );
   }
   else
   {
      SPRINTF_P( str, PSTR( "An %s approaches! Command?" ), game->battle.enemy.name );
   }

   Battle_ShowMessage( game, str );
   Menu_Load( &( game->menu ), MENUINDEX_BATTLEMAIN );
   Menu_Draw( game );
   game->state = GAMESTATE_BATTLEMENUMAIN;
}

void Battle_Attack( Game_t* game )
{
   char msg[64];

   Menu_WipeCarat( game );
   SPRINTF_P( msg, PSTR( "You attack!" ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLEATTACKANIMATION;
   Battle_AnimateAttack( game );
}

void Battle_Spell( Game_t* game )
{
   char msg[64];

   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   SPRINTF_P( msg, PSTR( "You scream 'ABRA KEDAVRA!!', which somehow kills him." ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Item( Game_t* game )
{
   char msg[64];

   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   SPRINTF_P( msg, PSTR( "You throw spare change from your pocket, and he dies." ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Flee( Game_t* game )
{
   char msg[64];

   Menu_Wipe( game );
   Screen_WipeEnemy( game, 160, 40 );
   SPRINTF_P( msg, PSTR( "You sneak out without him noticing. How brave!" ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Collect( Game_t* game )
{
   uint16_t experience, gold;
   Enemy_t* enemy = &( game->battle.enemy );
   Player_t* player = &( game->player );
   char msg[64];

   Menu_Wipe( game );

   if ( enemy->experience < ( INT16_MAX - player->experience ) )
   {
      experience = enemy->experience;
   }
   else
   {
      experience = INT16_MAX - player->experience;
   }

   if ( enemy->gold < ( INT16_MAX - player->gold ) )
   {
      gold = enemy->gold;
   }
   else
   {
      gold = INT16_MAX - player->gold;
   }

   player->experience += experience;
   player->gold += gold;

   if ( experience == 0 && gold == 0 )
   {
      Battle_Done( game );
   }
   else if ( experience == 0 && gold > 0 )
   {
      SPRINTF_P( msg, PSTR( "You have gained %u gold." ), gold );
      Battle_ShowMessage( game, msg );
      game->state = GAMESTATE_BATTLECOLLECT;
   }
   else
   {
      if ( gold > 0 )
      {
         SPRINTF_P( msg, PSTR( "You have gained %u experience and %u gold." ), experience, gold );
         Battle_ShowMessage( game, msg );
      }
      else
      {
         SPRINTF_P( msg, PSTR( "You have gained %u experience." ), experience );
         Battle_ShowMessage( game, msg );
      }

      // TODO: check for a level-up
      game->state = GAMESTATE_BATTLECOLLECT;
   }
}

void Battle_Done( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 36 );       // quick stats
   Screen_WipeTileMapSection( game, 144, 32, 112, 112 );    // enemy
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
   uint8_t payload;
   Enemy_t* enemy = &( game->battle.enemy );
   Player_t* player = &( game->player );
   char msg[128];

   // TODO: try slightly randomizing the payload, and also take the enemy's stats into account
   payload = player->stats.AttackPower;
   if ( payload > enemy->stats.HitPoints )
   {
      payload = enemy->stats.HitPoints;
   }

   enemy->stats.HitPoints -= payload;

   if ( enemy->stats.HitPoints == 0 )
   {
      Screen_WipeEnemy( game, 160, 40 );
      SPRINTF_P( msg,
                 PSTR( "The %s's hit points have been reduced by %u. You have defeated the %s!" ),
                 enemy->name,
                 payload,
                 enemy->name );
      Battle_ShowMessage( game, msg );

      if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) )
      {
         game->specialEnemyFlags ^= SPECIALENEMYFLAG_GREENDRAGON;
      }
      else if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) )
      {
         game->specialEnemyFlags ^= SPECIALENEMYFLAG_GOLEM;
      }
      else if ( Game_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT ) )
      {
         game->specialEnemyFlags ^= SPECIALENEMYFLAG_AXEKNIGHT;
      }

      game->state = GAMESTATE_BATTLERESULT;
   }
   else
   {
      SPRINTF_P( msg, PSTR( "The %s's hit points have been reduced by %u. Command?" ), enemy->name, payload );
      Battle_ShowMessage( game, msg );
      game->state = GAMESTATE_BATTLEMENUMAIN;
   }
}
