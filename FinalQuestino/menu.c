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
         cScreen_DrawRect( &( game->screen ), 16, 16, 76, 60, BLACK );
         snprintf( str, 9, "HP:%u", game->player.stats.HitPoints );
         cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
         snprintf( str, 9, "MP:%u", game->player.stats.MagicPoints );
         cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );
         snprintf( str, 9, " G:%u", game->player.gold );
         cScreen_DrawText( &( game->screen ), str, 24, 48, BLACK, WHITE );
         snprintf( str, 9, "EX:%u", game->player.experience );
         cScreen_DrawText( &( game->screen ), str, 24, 60, BLACK, WHITE );
         // menu items
         cScreen_DrawRect( &( game->screen ), 16, 88, 76, 88, BLACK );
         cScreen_DrawText( &( game->screen ), "TALK", 32, 96, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "STATUS", 32, 112, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "SEARCH", 32, 128, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "SPELL", 32, 144, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "ITEM", 32, 160, BLACK, WHITE );
         break;
   }

   cMenu_DrawCarat( game );
}

void cMenu_Wipe( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ), 16, 16, 76, 60 );
         cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ), 16, 88, 76, 88 );
         break;
   }
   
   cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                       game->player.position.x + game->player.spriteOffset.x,
                       game->player.position.y + game->player.spriteOffset.y );
}

void cMenu_Tic( cGame_t* game )
{
   cMenu_t* menu = &( game->menu );
   cBool_t showCaratCache = menu->showCarat;

   menu->caratSeconds += game->clock.frameSeconds;

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
         cScreen_DrawText( &( game->screen ), ">", 20, 96 + ( 16 * game->menu.optionIndex ), BLACK, WHITE );
         break;
   }
}

static void cMenu_WipeCarat( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_DrawText( &( game->screen ), " ", 20, 96 + ( 16 * game->menu.optionIndex ), BLACK, WHITE );
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
         cGame_ShowMessage( game, "Nobody's there." );
         break;
      case 1: // status
         cGame_ChangeState( game, cGameState_MapStatus );
         break;
      case 2: // search
         cGame_ShowMessage( game, "You didn't find anything." );
         break;
      case 3: // spell
         cGame_ShowMessage( game, "You don't know any spells." );
         break;
      case 4: // item
         cGame_ShowMessage( game, "You don't have any items." );
         break;
   }
}
