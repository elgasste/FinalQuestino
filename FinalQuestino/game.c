#include "game.h"
#include "random.h"
#include "battle.h"

static void Game_DrawMapStatus( Game_t* game );
static void Game_RollEncounter( Game_t* game, uint8_t encounterRate );
static Bool_t Game_OnAnySpecialEnemyTile( Game_t* game );
static Bool_t Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );

void Game_Init( Game_t* game )
{
   game->paletteIndex = 0;
   game->tileMapIndex = 77; // Tantegel throne room
   game->specialEnemyFlags = 0xFF;
   game->treasureFlags = 0xFFFF;

   Screen_Init( &( game->screen ) );
   Screen_LoadMapPalette( &( game->screen ), game->paletteIndex );
   Screen_Begin( &( game->screen ) );

   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );

   TileMap_Init( &( game->tileMap ) );
   TileTexture_LoadTileTextures( &( game->tileMap ) );

   Player_Init( &( game->player ) );
   game->player.sprite.direction = Direction_Up;
   game->player.sprite.frameSeconds = 0.2f;
   game->player.position.x = ( MAP_TILE_SIZE * 8 ) + 2;
   game->player.position.y = ( MAP_TILE_SIZE * 6 ) + 4;

   game->state = GameState_Init;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Input_Handle( game );

   switch( game->state )
   {
      case GameState_Init:
         Game_ChangeState( game, GameState_Map );
         break;
      case GameState_Map:
         Physics_Tic( game );
         break;
      case GameState_MapMenu:
         Menu_Tic( game );
         break;
   }
}

void Game_Refresh( Game_t* game )
{
   if ( game->state == GameState_Map )
   {
      TileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );
      Screen_DrawTileMap( game );

      if ( game->tileMap.spriteCount > 0 )
      {
         Screen_DrawMapSprites( game );
      }

      Screen_DrawPlayer( game );
      Physics_UpdateTileIndexCache( game );
   }
}

void Game_ChangeState( Game_t *game, GameState_t newState )
{
   switch( game->state )
   {
      case GameState_Init:
         if ( newState == GameState_Map )
         {
            game->state = newState;
            Game_Refresh( game );
         }
         break;
      case GameState_Map:
         switch( newState )
         {
            case GameState_MapMenu:
               game->state = newState;
               Menu_Load( &( game->menu ), MenuIndex_Map );
               Menu_Draw( game );
               break;
            case GameState_Battle:
               game->state = newState;
               Screen_WipePlayer( game );
               Battle_Start( game );
               break;
         }
         break;
      case GameState_MapMenu:
         switch( newState )
         {
            case GameState_Map:
            case GameState_MapMessage:
               game->state = newState;
               Menu_Wipe( game );
               break;
            case GameState_MapStatus:
               game->state = newState;
               Menu_Wipe( game );
               Game_DrawMapStatus( game );
               break;
         }
         break;
      case GameState_MapMessage:
         if ( newState == GameState_Map )
         {
            game->state = newState;
            Game_WipeMessage( game );
         }
         break;
      case GameState_MapStatus:
         if ( newState == GameState_Map )
         {
            game->state = newState;
            Screen_WipeTileMapSection( game, 16, 16, 112, 96 );
         }
         break;
      case GameState_Battle:
         if ( newState == GameState_Map )
         {
            game->state = newState;
            Battle_Done( game );
            Screen_DrawPlayer( game );
         }
         break;
   }
}

void Game_SteppedOnTile( Game_t* game, uint16_t tileIndex )
{
   uint8_t i, tileFlagIndex, tileFlags, tileSpeed, tileTextureIndex, encounterRate;
   uint8_t tile = game->tileMap.tiles[tileIndex];
   uint16_t newTileIndex, newTileX, newTileY;

   game->tileMap.tileIndexCache = tileIndex;

   for ( i = 0; i < MAP_PORTAL_COUNT; i++ )
   {
      if ( ( game->tileMap.portals[i] >> 21 ) == tileIndex )
      {
         game->tileMapIndex = (uint8_t)( ( game->tileMap.portals[i] >> 11 ) & 0x3FF );
         newTileIndex = game->tileMap.portals[i] & 0x7FF;
         game->tileMap.tileIndexCache = newTileIndex;
         newTileY = newTileIndex / MAP_TILES_X;
         newTileX = newTileIndex - ( newTileY * MAP_TILES_X );
         game->player.position.x = (float)( ( newTileX * MAP_TILE_SIZE ) + ( ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) / 2 ) );
         game->player.position.y = (float)( ( newTileY * MAP_TILE_SIZE ) + ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) - COLLISION_PADDING );
         Game_Refresh( game );
         Random_Seed();
         return;
      }
   }

   tileFlagIndex = tile & 0xF1;
   tileFlags = game->tileMap.tileTextures[MIN_I( tileFlagIndex, 15 )].flags;
   tileSpeed = ( tileFlags & 0xC ) >> 2;

   switch( tileSpeed )
   {
      case 1: game->player.maxVelocity = PLAYERVELOCITY_SLOW; break;
      case 2: game->player.maxVelocity = PLAYERVELOCITY_SLOWER; break;
      case 3: game->player.maxVelocity = PLAYERVELOCITY_CRAWL; break;
      default: game->player.maxVelocity = PLAYERVELOCITY_NORMAL; break;
   }

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[Button_B].down )
   {
      return;
   }
#endif

   if ( Game_OnAnySpecialEnemyTile( game ) )
   {
      Game_ChangeState( game, GameState_Battle );
   }
   else if ( tile & MAP_TILE_FLAG_ENCOUNTERABLE )
   {
      tileTextureIndex = tile & 0x1F;
      tileFlags = game->tileMap.tileTextures[MIN_I( tileTextureIndex, 15 )].flags;
      encounterRate = tileFlags & 0x3;

      if ( encounterRate > 0 )
      {
         Game_RollEncounter( game, encounterRate );
      }
   }
}

void Game_ShowMessage( Game_t* game, const char* message )
{
   Screen_DrawRect( &( game->screen ), 48, 160, 224, 64, DARKGRAY );
   Screen_DrawWrappedText( &( game->screen ), message, 56, 168, 26, 8, DARKGRAY, WHITE );
}

void Game_WipeMessage( Game_t* game )
{
   if ( game->state == GameState_Map )
   {
      Screen_WipeTileMapSection( game, 48, 160, 224, 64 );
      Screen_DrawMapSprites( game );
      Screen_DrawPlayer( game );
   }
}

static void Game_DrawMapStatus( Game_t* game )
{
   Player_t* player = &( game->player );
   char str[14];

   Screen_DrawRect( &( game->screen ), 16, 16, 112, 96, DARKGRAY );

   snprintf( str, 14, "Lvl: %u", Player_GetLevel( player ) );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   snprintf( str, 14, " HP: %u/%u", player->stats.HitPoints, player->stats.MaxHitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );
   snprintf( str, 14, " MP: %u/%u", player->stats.MagicPoints, player->stats.MaxMagicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 48, DARKGRAY, WHITE );
   snprintf( str, 14, "Atk: %u", player->stats.AttackPower );
   Screen_DrawText( &( game->screen ), str, 24, 60, DARKGRAY, WHITE );
   snprintf( str, 14, "Def: %u", player->stats.DefensePower );
   Screen_DrawText( &( game->screen ), str, 24, 72, DARKGRAY, WHITE );
   snprintf( str, 14, "Agl: %u", player->stats.Agility );
   Screen_DrawText( &( game->screen ), str, 24, 84, DARKGRAY, WHITE );
   snprintf( str, 14, "Exp: %u", player->experience );
   Screen_DrawText( &( game->screen ), str, 24, 96, DARKGRAY, WHITE );
}

static void Game_RollEncounter( Game_t* game, uint8_t encounterRate )
{
   Bool_t spawnEncounter;

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[Button_B].down )
   {
      return;
   }
#endif

   switch( encounterRate )
   {
      case 1: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_LOW; break;
      case 2: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_MEDIUM; break;
      case 3: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_HIGH; break;
      default: return;
   }

   if ( spawnEncounter )
   {
      Game_ChangeState( game, GameState_Battle );
   }
}

Bool_t Game_OnSpecialEnemyTile( Game_t* game, uint8_t specialEnemyId )
{
   switch( specialEnemyId )
   {
      case SPECIALENEMYID_GREENDRAGON:
         return game->tileMapIndex == SPECIALENEMYMAP_GREENDRAGON &&
                game->tileMap.tileIndexCache == SPECIALENEMYTILE_GREENDRAGON &&
                ( game->specialEnemyFlags & SPECIALENEMYFLAG_GREENDRAGON ) != 0;
      case SPECIALENEMYID_GOLEM:
         return game->tileMapIndex == SPECIALENEMYMAP_GOLEM &&
                game->tileMap.tileIndexCache == SPECIALENEMYTILE_GOLEM &&
                ( game->specialEnemyFlags & SPECIALENEMYFLAG_GOLEM ) != 0;
      case SPECIALENEMYID_AXEKNIGHT:
         return game->tileMapIndex == SPECIALENEMYMAP_AXEKNIGHT &&
                game->tileMap.tileIndexCache == SPECIALENEMYTILE_AXEKNIGHT &&
                ( game->specialEnemyFlags & SPECIALENEMYFLAG_AXEKNIGHT ) != 0;
   }

   return False;
}

static Bool_t Game_OnAnySpecialEnemyTile( Game_t* game )
{
   return Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) ||
          Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) ||
          Game_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT );
}

void Game_SearchMapTile( Game_t* game )
{
   uint8_t x, y;
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game->tileMapIndex, game->tileMap.tileIndexCache );

   if ( treasureFlag && ( game->treasureFlags & treasureFlag ) )
   {
      if ( Game_CollectTreasure( game, treasureFlag ) )
      {
         Game_ShowMapMessage( game, "Time to quit your day job!" );

         y = ( uint8_t )( game->tileMap.tileIndexCache / MAP_TILES_X );
         x = ( uint8_t )( game->tileMap.tileIndexCache - ( y * MAP_TILES_X ) );
         Screen_WipeTileMapSection( game, (float)x * MAP_TILE_SIZE, (float)y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE );
         Screen_DrawPlayer( game );
      }
      else
      {
         Game_ShowMapMessage( game, "Can't carry any more of these." );
      }
   }
   else
   {
      Game_ShowMapMessage( game, "You didn't find anything." );
   }
}

static Bool_t Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   // TODO: check if we can carry any more of whatever this is
   game->treasureFlags ^= treasureFlag;
   return True;
}

void Game_ShowMapMessage( Game_t* game, const char* message )
{
   Game_ChangeState( game, GameState_MapMessage );
   Game_ShowMessage( game, message );
}
