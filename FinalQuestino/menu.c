#include "game.h"
#include "menu.h"

internal void Menu_SetItemFlags( Game_t* game );
internal void Menu_MapMenuSelect( Game_t* game );
internal void Menu_BattleMainSelect( Game_t* game );
internal void Menu_ItemSelect( Game_t* game );
internal void Menu_DrawMapMenu( Game_t* game );
internal void Menu_DrawBattleMainMenu( Game_t* game );
internal void Menu_DrawMapItemsMenu( Game_t* game );
internal uint16_t Menu_GetMapItemsMenuHeight( Game_t* game );

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
         game->menu.optionCount = GET_MAPUSEABLEITEM_COUNT( game->player.items );
         Menu_SetItemFlags( game );
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

void Menu_Wipe( Game_t* game, uint8_t menuIndex )
{
   switch( menuIndex )
   {
      case MENUINDEX_MAP: Screen_WipeTileMapSection( game, 16, 96, 76, 88, False ); break;
      case MENUINDEX_BATTLEMAIN: Screen_WipeTileMapSection( game, 16, 152, 76, 72, False ); break;
      case MENUINDEX_MAPITEMS: Screen_WipeTileMapSection( game, 112, 16, 160, Menu_GetMapItemsMenuHeight( game ), False); break;
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
      case MENUINDEX_MAP: x = 20; y = 104; break;
      case MENUINDEX_BATTLEMAIN: x = 20; y = 160; break;
      case MENUINDEX_MAPITEMS:
         if ( !GET_MAPUSEABLEITEM_COUNT( game->player.items ) ) return;
         x = 116; y = 24; lineHeight = 8; break;
   }

   Screen_DrawText( &( game->screen ), STR_MENU_CARAT, x, y + ( lineHeight * game->menu.selectedOption ), DARKGRAY, WHITE );
}

void Menu_WipeCarat( Game_t* game )
{
   uint16_t x = 0, y = 0, lineHeight = 16;

   switch( game->menu.index )
   {
      case MENUINDEX_MAP: x = 20; y = 104; break;
      case MENUINDEX_BATTLEMAIN: x = 20; y = 160; break;
      case MENUINDEX_MAPITEMS:
         if ( !GET_MAPUSEABLEITEM_COUNT( game->player.items ) ) return;
         x = 116; y = 24; lineHeight = 8; break;
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
      case MENUINDEX_MAPITEMS:
         if ( GET_MAPUSEABLEITEM_COUNT( game->player.items ) )
         {
            Menu_ItemSelect( game );
         }
         break;
   }
}

internal void Menu_SetItemFlags( Game_t* game )
{
   uint32_t items = game->player.items;
   uint8_t shift = 0;

   game->menu.itemFlags = 0;

   if ( game->menu.index == MENUINDEX_MAPITEMS )
   {
      if ( GET_ITEM_KEYCOUNT( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_KEY << shift; shift += 4; }
      if ( GET_ITEM_HERBCOUNT( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_HERB << shift; shift += 4; }
      if ( GET_ITEM_WINGCOUNT( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_WING << shift; shift += 4; }
      if ( GET_ITEM_FAIRYWATERCOUNT( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_FAIRYWATER << shift; shift += 4; }
      if ( GET_ITEM_HASRAINBOWDROP( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_RAINBOWDROP << shift; shift += 4; }
      if ( GET_ITEM_HASSILVERHARP( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_SILVERHARP << shift; shift += 4; }
      if ( GET_ITEM_HASFAIRYFLUTE( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_FAIRYFLUTE << shift; shift += 4; }
      if ( GET_ITEM_HASGWAELYNSLOVE( items ) ) { game->menu.itemFlags |= (uint64_t)ITEM_GWAELYNSLOVE << shift; }
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

internal void Menu_ItemSelect( Game_t* game )
{
   uint8_t itemId = ( game->menu.itemFlags >> ( game->menu.selectedOption * 4 ) ) & 0xF;
   Game_UseMapItem( game, itemId );
}

internal void Menu_DrawMapMenu( Game_t* game )
{
   char str[8];

   Screen_DrawRect( &( game->screen ), 16, 96, 76, 88, DARKGRAY );
   SPRINTF_P( str, PSTR( STR_MENU_TALK ) );
   Screen_DrawText( &( game->screen ), str, 32, 104, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_STATUS ) );
   Screen_DrawText( &( game->screen ), str, 32, 120, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_SEARCH ) );
   Screen_DrawText( &( game->screen ), str, 32, 136, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_SPELL ) );
   Screen_DrawText( &( game->screen ), str, 32, 152, DARKGRAY, WHITE );
   SPRINTF_P( str, PSTR( STR_MENU_ITEM ) );
   Screen_DrawText( &( game->screen ), str, 32, 168, DARKGRAY, WHITE );
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
   uint16_t textY = 24;
   uint32_t items = game->player.items;

   Screen_DrawRect( &( game->screen ), 112, 16, 160, Menu_GetMapItemsMenuHeight( game ), DARKGRAY );

   if ( GET_MAPUSEABLEITEM_COUNT( items ) )
   {
      if ( GET_ITEM_KEYCOUNT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_KEY ), GET_ITEM_KEYCOUNT( items ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HERBCOUNT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_HERB ), GET_ITEM_HERBCOUNT( items ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_WINGCOUNT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_WING ), GET_ITEM_WINGCOUNT( items ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_FAIRYWATERCOUNT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_FAIRYWATER ), GET_ITEM_FAIRYWATERCOUNT( items ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASRAINBOWDROP( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_RAINBOWDROP ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASSILVERHARP( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_SILVERHARP ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASFAIRYFLUTE( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_FAIRYFLUTE ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASGWAELYNSLOVE( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_GWAELYNSLOVE ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }

      textY += 8;
   }

   if ( GET_MAPNONUSEABLEITEM_COUNT( items ) )
   {
      if ( GET_ITEM_HASSTONEOFSUNLIGHT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_STONEOFSUNLIGHT ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASSTAFFOFRAIN( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_STAFFOFRAIN ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASTOKEN( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_TOKEN ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
      if ( GET_ITEM_HASSPHEREOFLIGHT( items ) )
      {
         SPRINTF_P( str, PSTR( STR_MENU_SPHEREOFLIGHT ) );
         Screen_DrawText( &( game->screen ), str, 128, textY, DARKGRAY, WHITE );
         textY += 8;
      }
   }
}

internal uint16_t Menu_GetMapItemsMenuHeight( Game_t* game )
{
   uint16_t h;
   uint8_t useableItems = GET_MAPUSEABLEITEM_COUNT( game->player.items );
   uint8_t nonUseableItems = GET_MAPNONUSEABLEITEM_COUNT( game->player.items );

   h = ( 8 * useableItems ) + 16;

   if ( nonUseableItems )
   {
      h += ( 8 * nonUseableItems );

      if ( useableItems )
      {
         h += 8;
      }
   }

   return h;
}
