#include "game.h"
#include "random.h"
#include "battle.h"

internal void Game_RollEncounter( Game_t* game, uint8_t encounterRate );
internal Bool_t Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );
internal void Game_OpenDoor( Game_t* game );

void Game_Init( Game_t* game )
{
   game->paletteIndex = 0;
   game->tileMapIndex = 77; // Tantegel throne room
   game->specialEnemyFlags = 0xFF;
   game->treasureFlags = 0xFFFF;
   game->doorFlags = 0xFFFF;

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
      case GAMESTATE_MAPMENUITEMS:
         Menu_Tic( game );
         break;
   }
}

void Game_RefreshMap( Game_t* game )
{
   TileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );
   Screen_DrawTileMap( game );
   Screen_DrawMapSprites( game );
   Screen_DrawPlayer( game );
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
   Screen_WipeTileMapSection( game, 48, 144, 224, 80, False );
}

void Game_ShowMapMenuMessage( Game_t* game, const char* message )
{
   Screen_DrawRect( &( game->screen ), 112, 136, 160, 48, DARKGRAY );
   Screen_DrawWrappedText( &( game->screen ), message, 120, 144, 18, 10, DARKGRAY, WHITE );
}

void Game_WipeMapMenuMessage( Game_t* game )
{
   Screen_WipeTileMapSection( game, 112, 136, 160, 48, False );
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
   Screen_WipeTileMapSection( game, 16, 16, 76, 60, False );
}

void Game_WipeMapStatus( Game_t* game )
{
   Screen_WipeTileMapSection( game, 16, 16, 112, 96, False );
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
   Menu_Wipe( game, MENUINDEX_MAP );
   SPRINTF_P( msg, PSTR( STR_TEMP_TALK ) );
   Game_ShowMessage( game, msg );
   game->state = GAMESTATE_MAPMESSAGE;
}

void Game_Status( Game_t* game )
{
   Player_t* player = &( game->player );
   char str[14];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game, MENUINDEX_MAP );
   
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
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game->tileMapIndex, game->tileMap.tileIndexCache );
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game, MENUINDEX_MAP );

   if ( treasureFlag && ( game->treasureFlags & treasureFlag ) )
   {
      if ( Game_CollectTreasure( game, treasureFlag ) )
      {
         Screen_WipeTileIndex( game, game->tileMap.tileIndexCache, False );
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
   uint16_t gold = 0;
   Bool_t collected = False;
   char itemStr[32];
   char msg[128];

   switch ( treasureFlag )
   {
      case 0x1:  // tantegel throne room, upper-right chest
         collected = Player_CollectItem( &( game->player ), ITEM_KEY );
         SPRINTF_P( itemStr, PSTR( STR_ITEM_AKEY ) );
         SPRINTF_P( msg, collected ? PSTR( STR_TREASURE_ITEMCOLLECTED ) : PSTR( STR_TREASURE_ITEMDENIED ), itemStr );
         break;
      case 0x2: gold = 120; break;  // tantegel throne room, lower-left chest
      case 0x4:  // tantegel throne room, lower-right chest
         collected = Player_CollectItem( &( game->player ), ITEM_HERB );
         SPRINTF_P( itemStr, PSTR( STR_ITEM_ANHERB ) );
         SPRINTF_P( msg, collected ? PSTR( STR_TREASURE_ITEMCOLLECTED ) : PSTR( STR_TREASURE_ITEMDENIED ), itemStr );
         break;
      case 0x8: gold = 10; break;  // tantegel ground floor, upper-left area, upper-left chest
      case 0x10: gold = 15; break;  // tantegel ground floor, upper-left area, lower-left chest
      case 0x20: gold = 12; break;  // tantegel ground floor, upper-left area, lower-right chest
      case 0x40: gold = 30; break;  // tantegel ground floor, lower-left area
      case 0x80:  // tantegel basement
         collected = Player_CollectItem( &( game->player ), ITEM_STONEOFSUNLIGHT );
         SPRINTF_P( itemStr, PSTR( STR_ITEM_THESTONEOFSUNLIGHT ) );
         SPRINTF_P( msg, collected ? PSTR( STR_TREASURE_ITEMCOLLECTED ) : PSTR( STR_TREASURE_ITEMDENIED ), itemStr );
         break;
      case 0x100:  // erdrick's cave
         // TODO: this is the only item we don't show in any menus, instead we need to
         // display a series of dialogs with the tablet's message
         collected = Player_CollectItem( &( game->player ), ITEM_TABLET );
         SPRINTF_P( itemStr, PSTR( STR_ITEM_ERDRICKSTABLET ) );
         SPRINTF_P( msg, collected ? PSTR( STR_TREASURE_ITEMCOLLECTED ) : PSTR( STR_TREASURE_ITEMDENIED ), itemStr );
         break;
      default:
         SPRINTF_P( msg, PSTR( STR_ITEM_ERR ) );
         break;
   }

   if ( gold > 0 )
   {
      collected = ( Player_CollectGold( &( game->player ), gold ) > 0 ) ? True : False;
      SPRINTF_P( msg, collected ? PSTR( STR_TREASURE_GOLDCOLLECTED ) : PSTR( STR_TREASURE_GOLDDENIED ), gold );
   }
   
   Game_ShowMessage( game, msg );

   if ( collected )
   {
      game->treasureFlags ^= treasureFlag;
   }

   return collected;
}

void Game_MapSpell( Game_t* game )
{
   char msg[64];

   Game_WipeMapQuickStats( game );
   Menu_Wipe( game, MENUINDEX_MAP );
   SPRINTF_P( msg, PSTR( STR_NOSPELLS ) );
   Game_ShowMessage( game, msg );
   game->state = GAMESTATE_MAPMESSAGE;
}

void Game_MapItem( Game_t* game )
{
   char msg[64];

   if ( GET_MAPUSEABLEITEM_COUNT( game->player.items ) || GET_MAPNONUSEABLEITEM_COUNT( game->player.items ) )
   {
      Menu_DrawCarat( game );
      Menu_Load( game, MENUINDEX_MAPITEMS );
      Menu_Draw( game );
      Menu_Reset( game );
      game->state = GAMESTATE_MAPMENUITEMS;  
   }
   else
   {
      SPRINTF_P( msg, PSTR( STR_MAP_NOMAPITEMS ) );
      Menu_DrawCarat( game );
      Game_ShowMapMenuMessage( game, msg );
      game->state = GAMESTATE_MAPITEMMESSAGE;
   }
}

void Game_UseMapItem( Game_t* game, uint8_t itemId )
{
   char msg[64];

   switch ( itemId )
   {
      case ITEM_KEY:
         Game_OpenDoor( game );
         break;
      default:
         SPRINTF_P( msg, PSTR( STR_MAP_MAPITEMCANNOTBEUSED ) );
         Game_ShowMapMenuMessage( game, msg );
         game->state = GAMESTATE_MAPITEMMESSAGE;
         break;
   }
}

internal void Game_OpenDoor( Game_t* game )
{
   uint32_t doorFlag;
   int16_t facingTileIndex = TileMap_GetPlayerFacingTileIndex( game->physics.tileIndexCache, game->player.sprite.direction );
   char msg[32];

   if ( facingTileIndex >= 0 && TileMap_TileHasSprite( &( game->tileMap ), 1, (uint16_t)facingTileIndex ) )
   {
      doorFlag = TileMap_GetDoorFlag( game->tileMapIndex, (uint16_t)facingTileIndex );

      if ( game->doorFlags & doorFlag )
      {
         Game_WipeMapQuickStats( game );
         Menu_Wipe( game, MENUINDEX_MAP );
         Menu_Wipe( game, MENUINDEX_MAPITEMS );
         game->doorFlags ^= doorFlag;
         SET_ITEM_KEYCOUNT( game->player.items, GET_ITEM_KEYCOUNT( game->player.items ) - 1 );
         Screen_WipeTileIndex( game, facingTileIndex, False );
         game->state = GAMESTATE_MAP;
         return;
      }
      else
      {
         SPRINTF_P( msg, PSTR( STR_MAP_NODOOR ) );
      }
   }
   else
   {
      SPRINTF_P( msg, PSTR( STR_MAP_NODOOR ) );
   }

   Game_ShowMapMenuMessage( game, msg );
   game->state = GAMESTATE_MAPITEMMESSAGE;
}
