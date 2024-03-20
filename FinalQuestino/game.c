#include "game.h"
#include "random.h"

static void cGame_DrawMapStatus( cGame_t* game );

void cGame_Init( cGame_t* game )
{
   game->paletteIndex = 0;
   game->tileTexturesIndex = 0;
   game->tileMapIndex = 23;

   cScreen_Init( &( game->screen ) );
   cScreen_LoadPalette( &( game->screen ), game->paletteIndex );
   cScreen_Begin( &( game->screen ) );

   cClock_Init( &( game->clock ) );
   cInput_Init( &( game->input ) );

   cTileMap_Init( &( game->tileMap ) );
   cTileMap_LoadTileTextures( &( game->tileMap ), game->tileTexturesIndex );

   cPlayer_Init( &( game->player ) );
   game->player.sprite.direction = cDirection_Down;
   game->player.sprite.frameSeconds = 0.2f;
   game->player.position.x = TILE_SIZE * 10;
   game->player.position.y = TILE_SIZE * 6;
   game->player.hitBoxSize.x = 12;
   game->player.hitBoxSize.y = 12;
   game->player.spriteOffset.x = -2;
   game->player.spriteOffset.y = -4;

   game->state = cGameState_Init;

   cRandom_Seed();
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
      cScreen_DrawTileMap( &( game->screen ), &( game->tileMap ) );
      cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                          game->player.position.x + game->player.spriteOffset.x,
                          game->player.position.y + game->player.spriteOffset.y );
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
         if ( newState == cGameState_MapMenu )
         {
            game->state = newState;
            cMenu_Load( &( game->menu ), cMenuIndex_Map );
            cMenu_Draw( game );
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
            cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ), 48, 128, 224, 96 );
         }
         break;
      case cGameState_MapStatus:
         if ( newState == cGameState_Map )
         {
            game->state = newState;
            cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ), 16, 16, 112, 96 );
         }
         break;
   }
}

void cGame_SteppedOnTile( cGame_t* game, uint16_t tileIndex )
{
   uint8_t i;
   uint8_t tile = game->tileMap.tiles[tileIndex];
   uint16_t newTileIndex, newTileX, newTileY;

   for ( i = 0; i < PORTAL_COUNT; i++ )
   {
      if ( ( game->tileMap.portals[i] >> 21 ) == tileIndex )
      {
         game->tileMapIndex = ( game->tileMap.portals[i] >> 11 ) & 0x3FF;
         newTileIndex = game->tileMap.portals[i] & 0x7FF;
         newTileY = newTileIndex / TILES_X;
         newTileX = newTileIndex - ( newTileY * TILES_X );
         game->player.position.x = ( newTileX * TILE_SIZE ) + ( ( TILE_SIZE - game->player.hitBoxSize.x ) / 2 );
         game->player.position.y = ( newTileY * TILE_SIZE ) + ( TILE_SIZE - game->player.hitBoxSize.y ) - COLLISION_PADDING;
         cGame_Refresh( game );
         return;
      }
   }

  if ( tile & TILE_DAMAGE_FLAG )
  {
     // TODO: inflict damage
  }

  if ( tile & TILE_ENCOUNTERABLE_FLAG )
  {
     // TODO: roll for encounter
  }
}

void cGame_ShowMessage( cGame_t* game, const char* message )
{
   cGame_ChangeState( game, cGameState_MapMessage );
   cScreen_DrawRect( &( game->screen ), 48, 128, 224, 96, BLACK );
   cScreen_DrawWrappedText( &( game->screen ), message, 56, 136, 26, 8, BLACK, WHITE );
}

static void cGame_DrawMapStatus( cGame_t* game )
{
   cPlayer_t* player = &( game->player );
   char str[14];

   cScreen_DrawRect( &( game->screen ), 16, 16, 112, 96, BLACK );

   snprintf( str, 13, "Lvl: %u", cPlayer_GetLevel( player ) );
   cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
   snprintf( str, 13, " HP: %u/%u", player->stats.HitPoints, player->stats.MaxHitPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );
   snprintf( str, 13, " MP: %u/%u", player->stats.MagicPoints, player->stats.MaxMagicPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 48, BLACK, WHITE );
   snprintf( str, 13, "Atk: %u", player->stats.AttackPower );
   cScreen_DrawText( &( game->screen ), str, 24, 60, BLACK, WHITE );
   snprintf( str, 13, "Def: %u", player->stats.DefensePower );
   cScreen_DrawText( &( game->screen ), str, 24, 72, BLACK, WHITE );
   snprintf( str, 13, "Agl: %u", player->stats.Agility );
   cScreen_DrawText( &( game->screen ), str, 24, 84, BLACK, WHITE );
   snprintf( str, 13, "Exp: %u", player->experience );
   cScreen_DrawText( &( game->screen ), str, 24, 96, BLACK, WHITE );
}
