#include "game.h"

static void Menu_DrawCarat( Game_t* game );
static void Menu_WipeCarat( Game_t* game );
static void Menu_MapMenuSelect( Game_t* game );

void Menu_Load( Menu_t* menu, MenuIndex_t index )
{
   menu->index = index;
   menu->optionIndex = 0;
   menu->caratSeconds = 0;
   menu->showCarat = True;

   switch( index )
   {
      case MenuIndex_Map:
         menu->optionCount = 5;
         break;
   }
}

void Menu_Draw( Game_t* game )
{
   char str[10];

   switch( game->menu.index )
   {
      case MenuIndex_Map:
         // quick stats
         Screen_DrawRect( &( game->screen ), 16, 16, 76, 60, DARKGRAY );
         snprintf( str, 10, "HP:%u", game->player.stats.HitPoints );
         Screen_DrawText( &( game->screen ), str, 24, 24, DARKGRAY, WHITE );
         snprintf( str, 10, "MP:%u", game->player.stats.MagicPoints );
         Screen_DrawText( &( game->screen ), str, 24, 36, DARKGRAY, WHITE );
         snprintf( str, 10, " G:%u", game->player.gold );
         Screen_DrawText( &( game->screen ), str, 24, 48, DARKGRAY, WHITE );
         snprintf( str, 10, "EX:%u", game->player.experience );
         Screen_DrawText( &( game->screen ), str, 24, 60, DARKGRAY, WHITE );
         // menu items
         Screen_DrawRect( &( game->screen ), 16, 88, 76, 88, DARKGRAY );
         Screen_DrawText( &( game->screen ), "TALK", 32, 96, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), "STATUS", 32, 112, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), "SEARCH", 32, 128, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), "SPELL", 32, 144, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), "ITEM", 32, 160, DARKGRAY, WHITE );
         break;
   }

   Menu_DrawCarat( game );
}

void Menu_Wipe( Game_t* game )
{
   switch( game->menu.index )
   {
      case MenuIndex_Map:
         Screen_WipeTileMapSection( game, 16, 16, 76, 60 );
         Screen_WipeTileMapSection( game, 16, 88, 76, 88 );
         break;
   }
   
   Screen_DrawMapSprites( game );
   Screen_DrawPlayer( game );
}

void Menu_Tic( Game_t* game )
{
   Menu_t* menu = &( game->menu );
   Bool_t showCaratCache = menu->showCarat;

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
         Menu_DrawCarat( game );
      }
      else
      {
         Menu_WipeCarat( game );
      }
   }
}

static void Menu_DrawCarat( Game_t* game )
{
   switch( game->menu.index )
   {
      case MenuIndex_Map:
         Screen_DrawText( &( game->screen ), ">", 20, 96 + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
         break;
   }
}

static void Menu_WipeCarat( Game_t* game )
{
   switch( game->menu.index )
   {
      case MenuIndex_Map:
         Screen_DrawText( &( game->screen ), " ", 20, 96 + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
         break;
   }
}

void Menu_ScrollDown( Game_t* game )
{
   Menu_WipeCarat( game );
   game->menu.showCarat = True;
   game->menu.caratSeconds = 0;
   game->menu.optionIndex++;

   if ( game->menu.optionIndex >= game->menu.optionCount )
   {
      game->menu.optionIndex = 0;
   }

   Menu_DrawCarat( game );
}

void Menu_ScrollUp( Game_t* game )
{
   Menu_WipeCarat( game );
   game->menu.showCarat = True;
   game->menu.caratSeconds = 0;

   if ( game->menu.optionIndex == 0 )
   {
      game->menu.optionIndex = game->menu.optionCount - 1;
   }
   else
   {
      game->menu.optionIndex--;
   }

   Menu_DrawCarat( game );
}

void Menu_Select( Game_t* game )
{
   switch( game->menu.index )
   {
      case MenuIndex_Map:
         Menu_MapMenuSelect( game );
         break;
   }
}

static void Menu_MapMenuSelect( Game_t* game )
{
   switch( game->menu.optionIndex )
   {
      case 0: // talk
         Game_ShowMapMessage( game, "Nobody's there." );
         break;
      case 1: // status
         Game_ChangeState( game, GameState_MapStatus );
         break;
      case 2: // search
         Game_SearchMapTile( game );
         break;
      case 3: // spell
         Game_ShowMapMessage( game, "You don't know any spells." );
         break;
      case 4: // item
         Game_ShowMapMessage( game, "You don't have any items." );
         break;
   }
}
