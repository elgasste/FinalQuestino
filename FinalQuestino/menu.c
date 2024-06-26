#include "game.h"

static void cMenu_DrawCarat( cGame_t* game );
static void cMenu_WipeCarat( cGame_t* game );
static void cMenu_MapMenuSelect( cGame_t* game );

void cMenu_Load( cMenu_t* menu, cMenuIndex_t index )
{
   menu->index = index;
   menu->optionIndex = 0;
   menu->caratSeconds = 0;
   menu->showCarat = cTrue;

   switch( index )
   {
      case cMenuIndex_Map:
         menu->optionCount = 5;
         break;
   }
}

void cMenu_Draw( cGame_t* game )
{
   char str[10];

   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         // quick stats
         cScreen_DrawRect( &( game->screen ), 16, 16, 76, 60, DARKGRAY );
         snprintf( str, 10, "HP:%u", game->player.stats.HitPoints );
         cScreen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
         snprintf( str, 10, "MP:%u", game->player.stats.MagicPoints );
         cScreen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );
         snprintf( str, 10, " G:%u", game->player.gold );
         cScreen_DrawText( &( game->screen ), str, 24, 48, DARKGRAY, WHITE );
         snprintf( str, 10, "EX:%u", game->player.experience );
         cScreen_DrawText( &( game->screen ), str, 24, 60, DARKGRAY, WHITE );
         // menu items
         cScreen_DrawRect( &( game->screen ), 16, 88, 76, 88, DARKGRAY );
         cScreen_DrawText( &( game->screen ), "TALK", 32, 96, DARKGRAY, WHITE );
         cScreen_DrawText( &( game->screen ), "STATUS", 32, 112, DARKGRAY, WHITE );
         cScreen_DrawText( &( game->screen ), "SEARCH", 32, 128, DARKGRAY, WHITE );
         cScreen_DrawText( &( game->screen ), "SPELL", 32, 144, DARKGRAY, WHITE );
         cScreen_DrawText( &( game->screen ), "ITEM", 32, 160, DARKGRAY, WHITE );
         break;
   }

   cMenu_DrawCarat( game );
}

void cMenu_Wipe( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_WipeTileMapSection( game, 16, 16, 76, 60 );
         cScreen_WipeTileMapSection( game, 16, 88, 76, 88 );
         break;
   }
   
   cScreen_DrawMapSprites( game );
   cScreen_DrawPlayer( game );
}

void cMenu_Tic( cGame_t* game )
{
   cMenu_t* menu = &( game->menu );
   cBool_t showCaratCache = menu->showCarat;

   menu->caratSeconds += FRAME_SECONDS;

   while( menu->caratSeconds >= MENU_CARAT_BLINKRATE )
   {
      menu->caratSeconds -= MENU_CARAT_BLINKRATE;
      TOGGLE_BOOL( menu->showCarat );
   }

   if ( showCaratCache != menu->showCarat )
   {
      if ( menu->showCarat )
      {
         cMenu_DrawCarat( game );
      }
      else
      {
         cMenu_WipeCarat( game );
      }
   }
}

static void cMenu_DrawCarat( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_DrawText( &( game->screen ), ">", 20, 96 + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
         break;
   }
}

static void cMenu_WipeCarat( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_DrawText( &( game->screen ), " ", 20, 96 + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
         break;
   }
}

void cMenu_ScrollDown( cGame_t* game )
{
   cMenu_WipeCarat( game );
   game->menu.showCarat = cTrue;
   game->menu.caratSeconds = 0;
   game->menu.optionIndex++;

   if ( game->menu.optionIndex >= game->menu.optionCount )
   {
      game->menu.optionIndex = 0;
   }

   cMenu_DrawCarat( game );
}

void cMenu_ScrollUp( cGame_t* game )
{
   cMenu_WipeCarat( game );
   game->menu.showCarat = cTrue;
   game->menu.caratSeconds = 0;

   if ( game->menu.optionIndex == 0 )
   {
      game->menu.optionIndex = game->menu.optionCount - 1;
   }
   else
   {
      game->menu.optionIndex--;
   }

   cMenu_DrawCarat( game );
}

void cMenu_Select( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cMenu_MapMenuSelect( game );
         break;
   }
}

static void cMenu_MapMenuSelect( cGame_t* game )
{
   switch( game->menu.optionIndex )
   {
      case 0: // talk
         cGame_ShowMapMessage( game, "Nobody's there." );
         break;
      case 1: // status
         cGame_ChangeState( game, cGameState_MapStatus );
         break;
      case 2: // search
         cGame_SearchMapTile( game );
         break;
      case 3: // spell
         cGame_ShowMapMessage( game, "You don't know any spells." );
         break;
      case 4: // item
         cGame_ShowMapMessage( game, "You don't have any items." );
         break;
   }
}
