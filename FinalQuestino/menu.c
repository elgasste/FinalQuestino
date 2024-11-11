#include "game.h"
#include "menu.h"

internal void Menu_SetMapItemFlags( Game_t* game );
internal void Menu_MapMenuSelect( Game_t* game );
internal void Menu_BattleMainSelect( Game_t* game );
internal void Menu_DrawMapMenu( Game_t* game );
internal void Menu_DrawBattleMainMenu( Game_t* game );
internal void Menu_DrawMapItemsMenu( Game_t* game );

void Menu_Load( Game_t* game, uint8_t menuIndex )
{
   game->menu.index = menuIndex;
   Menu_Reset( game );

   switch( menuIndex )
   {
      case MENUINDEX_MAP:
         game->menu.optionCount = 5;
         break;
      case MENUINDEX_BATTLEMAIN:
         game->menu.optionCount = 4;
         break;
      case MENUINDEX_MAPITEMS:
         game->menu.optionCount = Player_GetMapItemCount( &( game->player ) );
         Menu_SetMapItemFlags( game );
         break;
   }
}

void Menu_Reset( Game_t* game )
{
   game->menu.selectedOption = 0;
   game->menu.caratSeconds = 0;
   game->menu.showCarat = True;
}

void Menu_Draw( Game_t* game )
{
   switch( game->menu.index )
   {
      case MENUINDEX_MAP: Menu_DrawMapMenu( game ); break;
      case MENUINDEX_BATTLEMAIN: Menu_DrawBattleMainMenu( game ); break;
      case MENUINDEX_MAPITEMS: Menu_DrawMapItemsMenu( game ); break;
   }

   Menu_DrawCarat( game );
}

void Menu_Wipe( Game_t* game )
{
   switch( game->menu.index )
   {
      case MENUINDEX_MAP: Screen_WipeTileMapSection( game, 16, 88, 76, 88, False ); break;
      case MENUINDEX_BATTLEMAIN: Screen_WipeTileMapSection( game, 16, 152, 76, 72, False ); break;
      case MENUINDEX_MAPITEMS: Screen_WipeTileMapSection( game, 112, 88, 164, 48, False ); break;
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

void Menu_DrawCarat( Game_t* game )
{
   uint16_t x = 0, y = 0, lineHeight = 16;

   switch( game->menu.index )
   {
      case MENUINDEX_MAP: x = 20; y = 96; break;
      case MENUINDEX_BATTLEMAIN: x = 20; y = 160; break;
      case MENUINDEX_MAPITEMS: x = 120; y = 96; lineHeight = 8; break;
   }

   Screen_DrawText( &( game->screen ), STR_MENU_CARAT, x, y + ( lineHeight * game->menu.selectedOption ), DARKGRAY, WHITE );
}

void Menu_WipeCarat( Game_t* game )
{
   uint16_t x = 0, y = 0, lineHeight = 16;

   switch( game->menu.index )
   {
      case MENUINDEX_MAP: x = 20; y = 96; break;
      case MENUINDEX_BATTLEMAIN: x = 20; y = 160; break;
      case MENUINDEX_MAPITEMS: x = 120; y = 96; lineHeight = 8; break;
   }

   Screen_DrawText( &( game->screen ), STR_MENU_BLANKCARAT, x, y + ( lineHeight * game->menu.selectedOption ), DARKGRAY, WHITE );
}

void Menu_ScrollDown( Game_t* game )
{
   Menu_WipeCarat( game );
   game->menu.showCarat = True;
   game->menu.caratSeconds = 0;
   game->menu.selectedOption++;

   if ( game->menu.selectedOption >= game->menu.optionCount )
   {
      game->menu.selectedOption = 0;
   }

   Menu_DrawCarat( game );
}

void Menu_ScrollUp( Game_t* game )
{
   Menu_WipeCarat( game );
   game->menu.showCarat = True;
   game->menu.caratSeconds = 0;

   if ( game->menu.selectedOption == 0 )
   {
      game->menu.selectedOption = game->menu.optionCount - 1;
   }
   else
   {
      game->menu.selectedOption--;
   }

   Menu_DrawCarat( game );
}

void Menu_Select( Game_t* game )
{
   switch( game->menu.index )
   {
      case MENUINDEX_MAP: Menu_MapMenuSelect( game ); break;
      case MENUINDEX_BATTLEMAIN: Menu_BattleMainSelect( game ); break;
      case MENUINDEX_MAPITEMS: Game_UseMapItem( game, ( game->menu.mapItemFlags >> ( game->menu.selectedOption * 2 ) ) & 0x3 ); break;
   }
}

internal void Menu_SetMapItemFlags( Game_t* game )
{
   uint32_t items = game->player.items;
   uint8_t shift = 0;

   if ( GET_ITEM_KEYCOUNT( items ) )
   {
      game->menu.mapItemFlags = ITEM_KEY;
      shift += 2;
   }
   if ( GET_ITEM_HERBCOUNT( items ) )
   {
      game->menu.mapItemFlags |= ( ITEM_HERB << shift );
      shift += 2;
   }
   if ( GET_ITEM_WINGCOUNT( items ) )
   {
      game->menu.mapItemFlags |= ( ITEM_WING << shift );
      shift += 2;
   }
   if ( GET_ITEM_FAIRYWATERCOUNT( items ) )
   {
      game->menu.mapItemFlags |= ( ITEM_FAIRYWATER << shift );
   }
}

internal void Menu_MapMenuSelect( Game_t* game )
{
   switch( game->menu.selectedOption )
   {
      case 0: Game_Talk( game ); break;
      case 1: Game_Status( game ); break;
      case 2: Game_Search( game ); break;
      case 3: Game_MapSpell( game ); break;
      case 4: Game_MapItem( game ); break;
   }
}

internal void Menu_BattleMainSelect( Game_t* game )
{
   switch ( game->menu.selectedOption )
   {
      case 0: Battle_Attack( game ); break;
      case 1: Battle_Spell( game ); break;
      case 2: Battle_Item( game ); break;
      case 3: Battle_Flee( game ); break;
   }
}

internal void Menu_DrawMapMenu( Game_t* game )
{
   char str[8];

   Screen_DrawRect( &( game->screen ), 16, 88, 76, 88, DARKGRAY );
   SPRINTF_P( str, PSTR( STR_MENU_TALK ) );
   Screen_DrawText( &( game->screen ), str, 32, 96, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_STATUS ) );
   Screen_DrawText( &( game->screen ), str, 32, 112, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_SEARCH ) );
   Screen_DrawText( &( game->screen ), str, 32, 128, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_SPELL ) );
   Screen_DrawText( &( game->screen ), str, 32, 144, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_ITEM ) );
   Screen_DrawText( &( game->screen ), str, 32, 160, DARKGRAY, WHITE );
}

internal void Menu_DrawBattleMainMenu( Game_t* game )
{
   char str[8];

   Screen_DrawRect( &( game->screen ), 16, 152, 76, 72, DARKGRAY );
   SPRINTF_P( str, PSTR( STR_MENU_ATTACK ) );
   Screen_DrawText( &( game->screen ), str, 32, 160, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_SPELL ) );
   Screen_DrawText( &( game->screen ), str, 32, 176, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_ITEM ) );
   Screen_DrawText( &( game->screen ), str, 32, 192, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_FLEE ) );
   Screen_DrawText( &( game->screen ), str, 32, 208, DARKGRAY, WHITE );
}

internal void Menu_DrawMapItemsMenu( Game_t* game )
{
   char str[32];
   uint16_t textY = 96;

   Screen_DrawRect( &( game->screen ), 112, 88, 164, 48, DARKGRAY );

   if ( GET_ITEM_KEYCOUNT( game->player.items ) )
   {
      SPRINTF_P( str, PSTR( STR_MENU_KEY ), GET_ITEM_KEYCOUNT( game->player.items ) );
      Screen_DrawText( &( game->screen ), str, 136, textY, DARKGRAY, WHITE );
      textY += 8;
   }

   if ( GET_ITEM_HERBCOUNT( game->player.items ) )
   {
      SPRINTF_P( str, PSTR( STR_MENU_HERB ), GET_ITEM_HERBCOUNT( game->player.items ) );
      Screen_DrawText( &( game->screen ), str, 136, textY, DARKGRAY, WHITE );
      textY += 8;
   }

   if ( GET_ITEM_WINGCOUNT( game->player.items ) )
   {
      SPRINTF_P( str, PSTR( STR_MENU_WING ), GET_ITEM_WINGCOUNT( game->player.items ) );
      Screen_DrawText( &( game->screen ), str, 136, textY, DARKGRAY, WHITE );
      textY += 8;
   }

   if ( GET_ITEM_FAIRYWATERCOUNT( game->player.items ) )
   {
      SPRINTF_P( str, PSTR( STR_MENU_FAIRYWATER ), GET_ITEM_FAIRYWATERCOUNT( game->player.items ) );
      Screen_DrawText( &( game->screen ), str, 136, textY, DARKGRAY, WHITE );
   }
}
