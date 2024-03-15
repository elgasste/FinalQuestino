#include "game.h"

void cMenu_Load( cMenu_t* menu, cMenuIndex_t index )
{
   menu->index = index;
   menu->optionIndex = 0;
   menu->caratSeconds = 0;
   menu->showCarat = cFalse;
}

void cMenu_Draw( cGame_t* game )
{
   cScreen_DrawRect( &( game->screen ), TILE_SIZE, TILE_SIZE, TILE_SIZE * 5, TILE_SIZE * 6, BLACK );
   cScreen_DrawText( &( game->screen ), "hi!", TILE_SIZE + 8, TILE_SIZE + 16, BLACK, WHITE );
}

void cMenu_Wipe( cGame_t* game )
{
   cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ),
                               TILE_SIZE, TILE_SIZE, TILE_SIZE * 5, TILE_SIZE * 6 );
   cScreen_WipeTileMapSection( &( game->screen ), &( game->tileMap ),
                               game->player.position.x + game->player.spriteOffset.x,
                               game->player.position.y + game->player.spriteOffset.y,
                               SPRITE_SIZE, SPRITE_SIZE );
   cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                       game->player.position.x + game->player.spriteOffset.x,
                       game->player.position.y + game->player.spriteOffset.y );
}
