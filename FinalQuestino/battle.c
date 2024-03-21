#include "battle.h"
#include "game.h"

void cBattle_Start( cGame_t* game )
{
   char str[10];

   cScreen_DrawRect( &( game->screen ), 16, 16, 76, 36, BLACK );
   snprintf( str, 9, "HP:%u", game->player.stats.HitPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
   snprintf( str, 9, "MP:%u", game->player.stats.MagicPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );

   // TODO: draw this in a spiral pattern
   cScreen_DrawRect( &( game->screen ), 128, 32, 112, 112, BLACK );

   cGame_ShowMessage( game, "An enemy approaches! However, due to a lack of funding, we can't tell you what it is. Press A or B to get on with your life." );
}

void cBattle_Done( cGame_t* game )
{
   cScreen_WipeTileMapSection( game, 16, 16, 76, 36 );
   cScreen_WipeTileMapSection( game, 112, 32, 128, 112 );
   cGame_WipeMessage( game );
}
