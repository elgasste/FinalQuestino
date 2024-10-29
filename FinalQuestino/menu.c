#include "game.h"

internal void Menu_DrawCarat( Game_t* game );
internal void Menu_WipeCarat( Game_t* game );
internal void Menu_MapMenuSelect( Game_t* game );
internal void Menu_BattleMainSelect( Game_t* game );

void Menu_Load( Menu_t* menu, uint8_t index )
{
   menu->index = index;
   menu->optionIndex = 0;
   menu->caratSeconds = 0;
   menu->showCarat = True;

   switch( index )
   {
      case MENUINDEX_MAP:
         menu->optionCount = 5;
         break;
      case MENUINDEX_BATTLEMAIN:
         menu->optionCount = 4;
         break;
   }
}

void Menu_Draw( Game_t* game )
{
   switch( game->menu.index )
   {
      case MENUINDEX_MAP:
         Screen_DrawRect( &( game->screen ), 16, 88, 76, 88, DARKGRAY );
         Screen_DrawText( &( game->screen ), PSTR( "TALK" ), 32, 96, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "STATUS" ), 32, 112, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "SEARCH" ), 32, 128, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "SPELL" ), 32, 144, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "ITEM" ), 32, 160, DARKGRAY, WHITE );
         break;
      case MENUINDEX_BATTLEMAIN:
         Screen_DrawRect( &( game->screen ), 16, 152, 76, 72, DARKGRAY );
         Screen_DrawText( &( game->screen ), PSTR( "ATTACK" ), 32, 160, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "SPELL" ), 32, 176, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "ITEM" ), 32, 192, DARKGRAY, WHITE );
         Screen_DrawText( &( game->screen ), PSTR( "FLEE" ), 32, 208, DARKGRAY, WHITE );
   }

   Menu_DrawCarat( game );
}

void Menu_Wipe( Game_t* game )
{
   switch( game->menu.index )
   {
      case MENUINDEX_MAP:
         Screen_WipeTileMapSection( game, 16, 88, 76, 88 );
         break;
      case MENUINDEX_BATTLEMAIN:
         Screen_WipeTileMapSection( game, 16, 152, 76, 72 );
         break;
   }
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

internal void Menu_DrawCarat( Game_t* game )
{
   uint16_t x = 0, y = 0;

   switch( game->menu.index )
   {
      case MENUINDEX_MAP:
         x = 20;
         y = 96;
         break;
      case MENUINDEX_BATTLEMAIN:
         x = 20;
         y = 160;
         break;
   }

   Screen_DrawText( &( game->screen ), ">", x, y + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
}

internal void Menu_WipeCarat( Game_t* game )
{
   uint16_t x = 0, y = 0;

   switch( game->menu.index )
   {
      case MENUINDEX_MAP:
         x = 20;
         y = 96;
         break;
      case MENUINDEX_BATTLEMAIN:
         x = 20;
         y = 160;
         break;
   }

   Screen_DrawText( &( game->screen ), " ", x, y + ( 16 * game->menu.optionIndex ), DARKGRAY, WHITE );
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
      case MENUINDEX_MAP:
         Menu_MapMenuSelect( game );
         break;
      case MENUINDEX_BATTLEMAIN:
         Menu_BattleMainSelect( game );
         break;
   }
}

internal void Menu_MapMenuSelect( Game_t* game )
{
   switch( game->menu.optionIndex )
   {
      case 0: // talk
         Game_Talk( game );
         break;
      case 1: // status
         Game_Status( game );
         break;
      case 2: // search
         Game_Search( game );
         break;
      case 3: // spell
         Game_MapSpell( game );
         break;
      case 4: // item
         Game_MapItem( game );
         break;
   }
}

internal void Menu_BattleMainSelect( Game_t* game )
{
   switch ( game->menu.optionIndex )
   {
      case 0: // attack
         Battle_Attack( game );
         break;
      case 1: // spell
         Battle_Spell( game );
         break;
      case 2: // item
         Battle_Item( game );
         break;
      case 3: // flee
         Battle_Flee( game );
         break;
   }
}
