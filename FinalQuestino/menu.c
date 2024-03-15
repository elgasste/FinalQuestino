#include "game.h"

static void cMenu_DrawCarat( cGame_t* game );
static void cMenu_WipeCarat( cGame_t* game );

void cMenu_Load( cMenu_t* menu, cMenuIndex_t index )
{
   menu->index = index;
   menu->optionIndex = 0;
   menu->caratSeconds = 0;
   menu->showCarat = cTrue;

   switch( index )
   {
      case cMenuIndex_Map:
         menu->optionCount = 3;
         break;
   }
}

void cMenu_Draw( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_DrawRect( &( game->screen ), 16, 16, 72, 56, BLACK );
         cScreen_DrawText( &( game->screen ), "hi", 32, 24, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "hello", 32, 40, BLACK, WHITE );
         cScreen_DrawText( &( game->screen ), "howdy", 32, 56, BLACK, WHITE );
         break;
   }

   cMenu_DrawCarat( game );
}

void cMenu_Wipe( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ), 16, 16, 72, 56 );
         cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ),
                                     game->player.position.x + game->player.spriteOffset.x,
                                     game->player.position.y + game->player.spriteOffset.y,
                                     SPRITE_SIZE, SPRITE_SIZE );
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

   while( menu->caratSeconds >= MENU_CARATBLINKRATE )
   {
      menu->caratSeconds -= MENU_CARATBLINKRATE;
      menu->showCarat = ( menu->showCarat ) ? cFalse : cTrue;
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
         cScreen_DrawText( &( game->screen ), ">", 20, 24 + ( 16 * game->menu.optionIndex ), BLACK, WHITE );
         break;
   }
}

static void cMenu_WipeCarat( cGame_t* game )
{
   switch( game->menu.index )
   {
      case cMenuIndex_Map:
         cScreen_DrawText( &( game->screen ), " ", 20, 24 + ( 16 * game->menu.optionIndex ), BLACK, WHITE );
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
