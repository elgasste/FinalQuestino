#include "game.h"
#include "random.h"
#include "battle.h"

internal void Game_RollEncounter( Game_t* game, uint8_t encounterRate );
internal Bool_t Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );

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
   game->player.sprite.direction = DIRECTION_UP;
   game->player.position.x = ( MAP_TILE_SIZE * 8 ) + 2;
   game->player.position.y = ( MAP_TILE_SIZE * 6 ) + 4;

   game->state = GAMESTATE_READY;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Input_Handle( game );

   switch( game->state )
   {
      case GAMESTATE_READY:
         Game_RefreshMap( game );
         game->state = GAMESTATE_MAP;
         break;
      case GAMESTATE_MAP:
         Physics_Tic( game );
         break;
      case GAMESTATE_MAPMENU:
      case GAMESTATE_BATTLEMENUMAIN:
         Menu_Tic( game );
         break;
   }
}

void Game_RefreshMap( Game_t* game )
{
   TileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );
   Screen_DrawTileMap( game );
   Screen_DrawActors( game );
   Physics_UpdateTileIndexCache( game );
}

void Game_SteppedOnTile( Game_t* game, uint16_t tileIndex )
{
   uint8_t i, tileTextureIndex, tileFlags, tileSpeed, encounterRate;
   uint8_t tile = game->tileMap.tiles[tileIndex];
   uint16_t newTileIndex, newTileX, newTileY;

   game->tileMap.tileIndexCache = tileIndex;

   for ( i = 0; i < MAP_PORTAL_COUNT; i++ )
   {
      if ( GET_PORTAL_ORIGIN( game->tileMap.portals[i] ) == tileIndex )
      {
         game->tileMapIndex = (uint8_t)( GET_PORTAL_DEST_TILE_MAP_INDEX( game->tileMap.portals[i] ) );
         newTileIndex = GET_PORTAL_DEST_TILE_INDEX( game->tileMap.portals[i] );
         game->tileMap.tileIndexCache = newTileIndex;
         newTileY = newTileIndex / MAP_TILES_X;
         newTileX = newTileIndex - ( newTileY * MAP_TILES_X );
         game->player.position.x = (float)( ( newTileX * MAP_TILE_SIZE ) + ( ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) / 2 ) );
         game->player.position.y = (float)( ( newTileY * MAP_TILE_SIZE ) + ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) - COLLISION_PADDING );
         Game_RefreshMap( game );
         Random_Seed();
         return;
      }
   }

   tileTextureIndex = GET_TILE_TEXTURE_INDEX( tile );
   tileFlags = game->tileMap.tileTextures[MIN_I( tileTextureIndex, 15 )].flags;
   tileSpeed = GET_TILE_SPEED( tileFlags );

   switch( tileSpeed )
   {
      case 1: game->player.maxVelocity = PLAYERVELOCITY_SLOW; break;
      case 2: game->player.maxVelocity = PLAYERVELOCITY_SLOWER; break;
      case 3: game->player.maxVelocity = PLAYERVELOCITY_CRAWL; break;
      default: game->player.maxVelocity = PLAYERVELOCITY_NORMAL; break;
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.fastWalk )
   {
      game->player.maxVelocity = 96;
   }
   if ( g_debugFlags.noEncounters )
   {
      return;
   }
#endif

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[BUTTON_B].down )
   {
      return;
   }
#endif

   if ( Game_OnAnySpecialEnemyTile( game ) )
   {
      Battle_Start( game );
   }
   else if ( GET_TILE_ENCOUNTERABLE( tile ) )
   {
      tileTextureIndex = GET_TILE_TEXTURE_INDEX( tile );
      tileFlags = game->tileMap.tileTextures[MIN_I( tileTextureIndex, 15 )].flags;
      encounterRate = GET_ENCOUNTER_RATE( tileFlags );

      if ( encounterRate > 0 )
      {
         Game_RollEncounter( game, encounterRate );
      }
   }
}

void Game_ShowMessage( Game_t* game, const char* message )
{
   Screen_DrawRect( &( game->screen ), 48, 152, 224, 72, DARKGRAY );
   Screen_DrawWrappedText( &( game->screen ), message, 56, 160, 26, 9, DARKGRAY, WHITE );
}

void Game_WipeMessage( Game_t* game )
{
   Screen_WipeTileMapSection( game, 48, 152, 224, 72 );
}

void Game_ShowMapQuickStats( Game_t* game )
{
   char str[10];

   Screen_DrawRect( &( game->screen ), 16, 16, 76, 60, DARKGRAY );
   SPRINTF_P( str, PSTR( STR_MAP_QUICKSTATSHP ), game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_QUICKSTATSMP ), game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_QUICKSTATSGOLD ), game->player.gold );
   Screen_DrawText( &( game->screen ), str, 24, 48, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_QUICKSTATSEXP ), game->player.experience );
   Screen_DrawText( &( game->screen ), str, 24, 60, DARKGRAY, WHITE );
}

void Game_WipeMapQuickStats( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 76, 60 );
}

void Game_WipeMapStatus( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 112, 96 );
}

internal void Game_RollEncounter( Game_t* game, uint8_t encounterRate )
{
   Bool_t spawnEncounter;

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noEncounters )
   {
      return;
   }
#endif

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[BUTTON_B].down )
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
      Battle_Start( game );
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

Bool_t Game_OnAnySpecialEnemyTile( Game_t* game )
{
   return Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) ||
          Game_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) ||
          Game_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT );
}

void Game_Talk( Game_t* game )
{
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game );
   SPRINTF_P( msg, PSTR( STR_TEMP_TALK ) );
   Game_ShowMessage( game, msg );
   game->state = GAMESTATE_MAPMESSAGE;
}

void Game_Status( Game_t* game )
{
   Player_t* player = &( game->player );
   char str[14];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game );
   
   Screen_DrawRect( &( game->screen ), 16, 16, 112, 96, DARKGRAY );

   SPRINTF_P( str, PSTR( STR_MAP_STATUSLEVEL ), Player_GetLevel( player ) );
   Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSHP ), player->stats.hitPoints, player->stats.maxHitPoints );
   Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSMP ), player->stats.magicPoints, player->stats.maxMagicPoints );
   Screen_DrawText( &( game->screen ), str, 24, 48, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSATTACK ), player->stats.attackPower );
   Screen_DrawText( &( game->screen ), str, 24, 60, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSDEFENSE ), player->stats.defensePower );
   Screen_DrawText( &( game->screen ), str, 24, 72, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSAGILITY ), player->stats.agility );
   Screen_DrawText( &( game->screen ), str, 24, 84, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MAP_STATUSEXP ), player->experience );
   Screen_DrawText( &( game->screen ), str, 24, 96, DARKGRAY, WHITE );

   game->state = GAMESTATE_MAPSTATUS;
}

void Game_Search( Game_t* game )
{
   uint8_t x, y;
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game->tileMapIndex, game->tileMap.tileIndexCache );
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game );

   if ( treasureFlag && ( game->treasureFlags & treasureFlag ) )
   {
      if ( Game_CollectTreasure( game, treasureFlag ) )
      {
         SPRINTF_P( msg, PSTR( STR_TEMP_COLLECTTREASURE ) );
         Game_ShowMessage( game, msg );

         y = ( uint8_t )( game->tileMap.tileIndexCache / MAP_TILES_X );
         x = ( uint8_t )( game->tileMap.tileIndexCache - ( y * MAP_TILES_X ) );
         Screen_WipeTileMapSection( game, (float)x * MAP_TILE_SIZE, (float)y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE );
         Screen_DrawPlayer( game );
      }
      else
      {
         SPRINTF_P( msg, PSTR( STR_MAP_ITEMCAPACITY ) );
         Game_ShowMessage( game, msg );
      }
   }
   else
   {
      SPRINTF_P( msg, PSTR( STR_MAP_FOUNDNOTHING ) );
      Game_ShowMessage( game, msg );
   }

   game->state = GAMESTATE_MAPMESSAGE;
}

internal Bool_t Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   // TODO: check if we can carry any more of whatever this is
   game->treasureFlags ^= treasureFlag;
   return True;
}

void Game_MapSpell( Game_t* game )
{
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game );
   SPRINTF_P( msg, PSTR( STR_NOSPELLS ) );
   Game_ShowMessage( game, msg );
   game->state = GAMESTATE_MAPMESSAGE;
}

void Game_MapItem( Game_t* game )
{
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game );
   SPRINTF_P( msg, PSTR( STR_MAP_NOITEMS ) );
   Game_ShowMessage( game, msg );
   game->state = GAMESTATE_MAPMESSAGE;
}
