#include "game.h"
#include "random.h"
#include "battle.h"

static void cGame_DrawMapStatus( cGame_t* game );
static void cGame_RollEncounter( cGame_t* game, uint8_t encounterRate );
static cBool_t cGame_OnAnySpecialEnemyTile( cGame_t* game );
static cBool_t cGame_CollectTreasure( cGame_t* game, uint32_t treasureFlag );

void cGame_Init( cGame_t* game )
{
   game->paletteIndex = 0;
   game->tileMapIndex = 77; // Tantegel throne room
   game->specialEnemyFlags = 0xFF;
   game->treasureFlags = 0xFFFF;

   cScreen_Init( &( game->screen ) );
   cScreen_LoadMapPalette( &( game->screen ), game->paletteIndex );
   cScreen_Begin( &( game->screen ) );

   cClock_Init( &( game->clock ) );
   cInput_Init( &( game->input ) );

   cTileMap_Init( &( game->tileMap ) );
   cTileTexture_LoadTileTextures( &( game->tileMap ) );

   cPlayer_Init( &( game->player ) );
   game->player.sprite.direction = cDirection_Up;
   game->player.sprite.frameSeconds = 0.2f;
   game->player.position.x = ( MAP_TILE_SIZE * 8 ) + 2;
   game->player.position.y = ( MAP_TILE_SIZE * 6 ) + 4;

   game->state = cGameState_Init;
}

void cGame_Tic( cGame_t* game )
{
   cInput_Read( &( game->input ) );
   cInput_Handle( game );

   switch( game->state )
   {
      case cGameState_Init:
         cGame_ChangeState( game, cGameState_Map );
         break;
      case cGameState_Map:
         cPhysics_Tic( game );
         break;
      case cGameState_MapMenu:
         cMenu_Tic( game );
         break;
   }
}

void cGame_Refresh( cGame_t* game )
{
   if ( game->state == cGameState_Map )
   {
      cTileMap_LoadTileMap( &( game->tileMap ), game->tileMapIndex );
      cScreen_DrawTileMap( game );

      if ( game->tileMap.spriteCount > 0 )
      {
         cScreen_DrawMapSprites( game );
      }

      cScreen_DrawPlayer( game );
      cPhysics_UpdateTileIndexCache( game );
   }
}

void cGame_ChangeState( cGame_t *game, cGameState_t newState )
{
   switch( game->state )
   {
      case cGameState_Init:
         if ( newState == cGameState_Map )
         {
            game->state = newState;
            cGame_Refresh( game );
         }
         break;
      case cGameState_Map:
         switch( newState )
         {
            case cGameState_MapMenu:
               game->state = newState;
               cMenu_Load( &( game->menu ), cMenuIndex_Map );
               cMenu_Draw( game );
               break;
            case cGameState_Battle:
               game->state = newState;
               cScreen_WipePlayer( game );
               cBattle_Start( game );
               break;
         }
         break;
      case cGameState_MapMenu:
         switch( newState )
         {
            case cGameState_Map:
            case cGameState_MapMessage:
               game->state = newState;
               cMenu_Wipe( game );
               break;
            case cGameState_MapStatus:
               game->state = newState;
               cMenu_Wipe( game );
               cGame_DrawMapStatus( game );
               break;
         }
         break;
      case cGameState_MapMessage:
         if ( newState == cGameState_Map )
         {
            game->state = newState;
            cGame_WipeMessage( game );
         }
         break;
      case cGameState_MapStatus:
         if ( newState == cGameState_Map )
         {
            game->state = newState;
            cScreen_WipeTileMapSection( game, 16, 16, 112, 96 );
         }
         break;
      case cGameState_Battle:
         if ( newState == cGameState_Map )
         {
            game->state = newState;
            cBattle_Done( game );
            cScreen_DrawPlayer( game );
         }
         break;
   }
}

void cGame_SteppedOnTile( cGame_t* game, uint16_t tileIndex )
{
   uint8_t i, tileFlags, tileSpeed, encounterRate;
   uint8_t tile = game->tileMap.tiles[tileIndex];
   uint16_t newTileIndex, newTileX, newTileY;

   game->tileMap.tileIndexCache = tileIndex;

   for ( i = 0; i < MAP_PORTAL_COUNT; i++ )
   {
      if ( ( game->tileMap.portals[i] >> 21 ) == tileIndex )
      {
         game->tileMapIndex = ( game->tileMap.portals[i] >> 11 ) & 0x3FF;
         newTileIndex = game->tileMap.portals[i] & 0x7FF;
         game->tileMap.tileIndexCache = newTileIndex;
         newTileY = newTileIndex / MAP_TILES_X;
         newTileX = newTileIndex - ( newTileY * MAP_TILES_X );
         game->player.position.x = ( newTileX * MAP_TILE_SIZE ) + ( ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) / 2 );
         game->player.position.y = ( newTileY * MAP_TILE_SIZE ) + ( MAP_TILE_SIZE - PLAYER_HITBOX_SIZE ) - COLLISION_PADDING;
         cGame_Refresh( game );
         cRandom_Seed();
         return;
      }
   }

   tileFlags = game->tileMap.tileTextures[tile & 0x1F].flags;
   tileSpeed = ( tileFlags & 0xC ) >> 2;

   switch( tileSpeed )
   {
      case 1: game->player.maxVelocity = PLAYERVELOCITY_SLOW; break;
      case 2: game->player.maxVelocity = PLAYERVELOCITY_SLOWER; break;
      case 3: game->player.maxVelocity = PLAYERVELOCITY_CRAWL; break;
      default: game->player.maxVelocity = PLAYERVELOCITY_NORMAL; break;
   }

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[cButton_B].down )
   {
      return;
   }
#endif

   if ( cGame_OnAnySpecialEnemyTile( game ) )
   {
      cGame_ChangeState( game, cGameState_Battle );
   }
   else if ( tile & MAP_TILE_FLAG_ENCOUNTERABLE )
   {
      encounterRate = game->tileMap.tileTextures[tile & 0x1F].flags & 0x3;

      if ( encounterRate > 0 )
      {
         cGame_RollEncounter( game, encounterRate );
      }
   }
}

void cGame_ShowMessage( cGame_t* game, const char* message )
{
   cScreen_DrawRect( &( game->screen ), 48, 160, 224, 64, BLACK );
   cScreen_DrawWrappedText( &( game->screen ), message, 56, 168, 26, 8, BLACK, WHITE );
}

void cGame_WipeMessage( cGame_t* game )
{
   if ( game->state == cGameState_Map )
   {
      cScreen_WipeTileMapSection( game, 48, 160, 224, 64 );
      cScreen_DrawMapSprites( game );
      cScreen_DrawPlayer( game );
   }
}

static void cGame_DrawMapStatus( cGame_t* game )
{
   cPlayer_t* player = &( game->player );
   char str[14];

   cScreen_DrawRect( &( game->screen ), 16, 16, 112, 96, BLACK );

   snprintf( str, 14, "Lvl: %u", cPlayer_GetLevel( player ) );
   cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
   snprintf( str, 14, " HP: %u/%u", player->stats.HitPoints, player->stats.MaxHitPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );
   snprintf( str, 14, " MP: %u/%u", player->stats.MagicPoints, player->stats.MaxMagicPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 48, BLACK, WHITE );
   snprintf( str, 14, "Atk: %u", player->stats.AttackPower );
   cScreen_DrawText( &( game->screen ), str, 24, 60, BLACK, WHITE );
   snprintf( str, 14, "Def: %u", player->stats.DefensePower );
   cScreen_DrawText( &( game->screen ), str, 24, 72, BLACK, WHITE );
   snprintf( str, 14, "Agl: %u", player->stats.Agility );
   cScreen_DrawText( &( game->screen ), str, 24, 84, BLACK, WHITE );
   snprintf( str, 14, "Exp: %u", player->experience );
   cScreen_DrawText( &( game->screen ), str, 24, 96, BLACK, WHITE );
}

static void cGame_RollEncounter( cGame_t* game, uint8_t encounterRate )
{
   cBool_t spawnEncounter;

#if defined( DEBUG_NOENCOUNTERSONB )
   if ( game->input.buttonStates[cButton_B].down )
   {
      return;
   }
#endif

   switch( encounterRate )
   {
      case 1: spawnEncounter = cRandom_Percent() <= ENCOUNTERRATE_LOW; break;
      case 2: spawnEncounter = cRandom_Percent() <= ENCOUNTERRATE_MEDIUM; break;
      case 3: spawnEncounter = cRandom_Percent() <= ENCOUNTERRATE_HIGH; break;
      default: return;
   }

   if ( spawnEncounter )
   {
      cGame_ChangeState( game, cGameState_Battle );
   }
}

cBool_t cGame_OnSpecialEnemyTile( cGame_t* game, uint8_t specialEnemyId )
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

   return cFalse;
}

static cBool_t cGame_OnAnySpecialEnemyTile( cGame_t* game )
{
   return cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_GREENDRAGON ) ||
          cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_GOLEM ) ||
          cGame_OnSpecialEnemyTile( game, SPECIALENEMYID_AXEKNIGHT );
}

void cGame_SearchMapTile( cGame_t* game )
{
   uint8_t x, y;
   uint32_t treasureFlag = cTileMap_GetTreasureFlag( game, game->tileMapIndex, game->tileMap.tileIndexCache );

   if ( treasureFlag && ( game->treasureFlags & treasureFlag ) )
   {
      if ( cGame_CollectTreasure( game, treasureFlag ) )
      {
         cGame_ShowMapMessage( game, "Time to quit your day job!" );

         y = game->tileMap.tileIndexCache / MAP_TILES_X;
         x = game->tileMap.tileIndexCache - ( y * MAP_TILES_X );
         cScreen_WipeTileMapSection( game, x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE );
         cScreen_DrawPlayer( game );
      }
      else
      {
         cGame_ShowMapMessage( game, "Can't carry any more of these." );
      }
   }
   else
   {
      cGame_ShowMapMessage( game, "You didn't find anything." );
   }
}

static cBool_t cGame_CollectTreasure( cGame_t* game, uint32_t treasureFlag )
{
   // TODO: check if we can carry any more of whatever this is
   game->treasureFlags ^= treasureFlag;
   return cTrue;
}

void cGame_ShowMapMessage( cGame_t* game, const char* message )
{
   cGame_ChangeState( game, cGameState_MapMessage );
   cGame_ShowMessage( game, message );
}
