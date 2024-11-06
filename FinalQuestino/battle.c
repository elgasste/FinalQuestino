#include "game.h"
#include "random.h"
#include "menu.h"

internal void Battle_AnimateStart( Game_t* game );
internal void Battle_ShowMessage( Game_t* game, const char* message );
internal void Battle_WipeMessage( Game_t* game );
internal void Battle_AnimateAttack( Game_t* game );
internal void Battle_AnimateFlee( Game_t* game );

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
   SPRINTF_P( str, PSTR( STR_BATTLE_QUICKSTATSHP ), game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_BATTLE_QUICKSTATSMP ), game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );

   if ( game->battle.enemy.indefiniteArticle == INDEFINITEARTICLE_A )
   {
      SPRINTF_P( str, PSTR( STR_BATTLE_APPROACHINDEFINITEA ), game->battle.enemy.name );
   }
   else
   {
      SPRINTF_P( str, PSTR( STR_BATTLE_APPROACHINDEFINITEAN ), game->battle.enemy.name );
   }

   Battle_ShowMessage( game, str );
   Menu_Load( game, MENUINDEX_BATTLEMAIN );
   Menu_Draw( game );
   Menu_Reset( game );
   game->state = GAMESTATE_BATTLEMENUMAIN;
}

void Battle_Attack( Game_t* game )
{
   char msg[64];

   Menu_WipeCarat( game );
   SPRINTF_P( msg, PSTR( STR_BATTLE_PLAYERATTACK ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLEATTACKANIMATION;
   Battle_AnimateAttack( game );
}

void Battle_Spell( Game_t* game )
{
   char msg[64];

   Menu_WipeCarat( game );
   Screen_WipeEnemy( game, 160, 40 );
   SPRINTF_P( msg, PSTR( STR_TEMP_SPELL ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Item( Game_t* game )
{
   char msg[64];

   Menu_WipeCarat( game );
   Screen_WipeEnemy( game, 160, 40 );
   SPRINTF_P( msg, PSTR( STR_TEMP_ITEM ) );
   Battle_ShowMessage( game, msg );
   game->state = GAMESTATE_BATTLERESULT;
}

void Battle_Flee( Game_t* game )
{
   char msg[32];

   Menu_WipeCarat( game );
   SPRINTF_P( msg, PSTR( STR_BATTLE_PLAYERFLEE ) );
   Battle_ShowMessage( game, msg );
   Battle_AnimateFlee( game );
}

void Battle_Collect( Game_t* game )
{
   uint16_t experience, gold;
   char msg[64];

   experience = Player_CollectExperience( &( game->player ), game->battle.enemy.experience );
   gold = Player_CollectGold( &( game->player ), game->battle.enemy.gold );

   if ( experience == 0 && gold == 0 )
   {
      Battle_Done( game );
   }
   else if ( experience == 0 && gold > 0 )
   {
      SPRINTF_P( msg, PSTR( STR_BATTLE_GOLDHAUL ), gold );
      Battle_ShowMessage( game, msg );
      game->state = GAMESTATE_BATTLECOLLECT;
   }
   else
   {
      if ( gold > 0 )
      {
         SPRINTF_P( msg, PSTR( STR_BATTLE_EXPERIENCEGOLDHAUL ), experience, gold );
         Battle_ShowMessage( game, msg );
      }
      else
      {
         SPRINTF_P( msg, PSTR( STR_BATTLE_EXPERIENCEHAUL ), experience );
         Battle_ShowMessage( game, msg );
      }

      // TODO: check for a level-up
      game->state = GAMESTATE_BATTLECOLLECT;
   }
}

void Battle_Done( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 36, False );       // quick stats
   Screen_WipeTileMapSection( game, 144, 32, 112, 112, False );    // enemy
   Menu_Wipe( game );
   Battle_WipeMessage( game );
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
   Screen_WipeTileMapSection( game, 96, 152, 208, 72, False );
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

internal void Battle_AnimateFlee( Game_t* game )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( game );
   Battle_WinAnimateFlee();
#else
   DELAY_MS( 1200 );
   Battle_ExecuteFlee( game );
#endif
}

void Battle_ExecuteAttack( Game_t* game )
{
   uint8_t payload;
   Enemy_t* enemy = &( game->battle.enemy );
   Player_t* player = &( game->player );
   char msg[128];

   // TODO: try slightly randomizing the payload, and also take the enemy's stats into account
   payload = player->stats.attackPower;

   if ( payload > enemy->stats.hitPoints )
   {
      payload = enemy->stats.hitPoints;
   }

   enemy->stats.hitPoints -= payload;

   if ( enemy->stats.hitPoints == 0 )
   {
      Screen_WipeEnemy( game, 160, 40 );
      SPRINTF_P( msg,
                 PSTR( STR_BATTLE_ATTACKENEMYDEATH ),
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
      SPRINTF_P( msg, PSTR( STR_BATTLE_ATTACKENEMY ), enemy->name, payload );
      Battle_ShowMessage( game, msg );
      Menu_Reset( game );
      game->state = GAMESTATE_BATTLEMENUMAIN;
   }
}

void Battle_ExecuteFlee( Game_t* game )
{
   BattleStats_t* playerStats = &( game->player.stats );
   BattleStats_t* enemyStats = &( game->battle.enemy.stats );
   Bool_t success = False;
   int16_t agilityDiff;
   uint8_t fleeChance;
   char msg[64];

   if ( !Game_OnAnySpecialEnemyTile( game ) && playerStats->agility > 0 )
   {
      if ( enemyStats->agility == 0 )
      {
         success = True;
      }
      else if ( playerStats->agility > enemyStats->agility ||
                ( enemyStats->agility - playerStats->agility ) <= BATTLE_FLEEAGILITYTHRESHOLD )
      {
         agilityDiff = (int16_t)playerStats->agility - enemyStats->agility;

         if ( agilityDiff > BATTLE_FLEEAGILITYTHRESHOLD )
         {
            success = True;
         }
         else
         {
            fleeChance = (uint8_t)( ( agilityDiff < 0 ) ?
                                    ( (float)( -agilityDiff ) / BATTLE_FLEEAGILITYTHRESHOLD ) * 100 :
                                    ( (float)agilityDiff / BATTLE_FLEEAGILITYTHRESHOLD ) * 100 );

            if ( Random_Percent() <= fleeChance )
            {
               success = True;
            }
         }
      }
   }

   if ( success )
   {
      Screen_WipeEnemy( game, 160, 40 );
      SPRINTF_P( msg, PSTR( STR_BATTLE_FLEESUCCESS ), game->battle.enemy.name );
      Battle_ShowMessage( game, msg );
      game->state = GAMESTATE_BATTLECOLLECT;
   }
   else
   {
      SPRINTF_P( msg, PSTR( STR_BATTLE_FLEEBLOCK ), game->battle.enemy.name );
      Battle_ShowMessage( game, msg );
      Menu_Reset( game );
      game->state = GAMESTATE_BATTLEMENUMAIN;
   }
}
