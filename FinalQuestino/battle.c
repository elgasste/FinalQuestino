#include "game.h"

static cBattle_AnimateStart( cGame_t* game );

void cBattle_Start( cGame_t* game )
{
   char str[10];

   cEnemy_Generate( &( game->battle.enemy ), 0 );

   cBattle_AnimateStart( game );

   // quick stats
   cScreen_DrawRect( &( game->screen ), 16, 16, 76, 36, BLACK );
   snprintf( str, 10, "HP:%u", game->player.stats.HitPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 24, BLACK, WHITE );
   snprintf( str, 10, "MP:%u", game->player.stats.MagicPoints );
   cScreen_DrawText( &( game->screen ), str, 24, 36, BLACK, WHITE );

   cGame_ShowMessage( game, "An enemy approaches! However, due to a lack of funding, we can't tell you what it is. Press A or B to get on with your life." );

   // MUFFINS: so this is the space we can work with. if we split it up into 8x8 tiles,
   // that's 10x12, or 120 tiles.
   cScreen_DrawRect( &( game->screen ), 144, 40, 80, 96, MAGENTA );
}

void cBattle_Done( cGame_t* game )
{
   cScreen_WipeTileMapSection( game, 16, 16, 76, 36 );
   cScreen_WipeTileMapSection( game, 128, 32, 112, 112 );
   cGame_WipeMessage( game );
}

static cBattle_AnimateStart( cGame_t* game )
{
   cScreen_DrawRect( &( game->screen ), 176, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 128, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 128, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 144, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 160, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 176, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 192, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 208, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 32, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 48, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 64, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 80, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 96, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 112, 16, 16, BLACK ); delay( 10 );
   cScreen_DrawRect( &( game->screen ), 224, 128, 16, 16, BLACK ); delay( 10 );
}
