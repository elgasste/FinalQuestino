#include "game.h"

#define COLLISION_PADDING 0.001f

void cPhysics_Init( cPhysics_t* physics )
{
  physics->spriteFrameCache = 0;
}

void cPhysics_MovePlayer( cGame_t* game )
{
  cVector2f_t prevPlayerPos;

  prevPlayerPos.x = game->player.position.x;
  prevPlayerPos.y = game->player.position.y;
  game->player.position.x += ( game->player.velocity.x * game->clock.frameSeconds );
  game->player.position.y += ( game->player.velocity.y * game->clock.frameSeconds );

  if ( game->player.position.x < 0 )
  {
    game->player.position.x = 0;
  }
  else if ( ( game->player.position.x + game->player.hitBoxSize.x ) >= ( TILES_X * TILE_SIZE ) )
  {
    game->player.position.x = ( TILES_X * TILE_SIZE ) - game->player.hitBoxSize.x - COLLISION_PADDING;
  }
  if ( game->player.position.y < 0 )
  {
    game->player.position.y = 0;
  }
  else if ( ( game->player.position.y + game->player.hitBoxSize.y ) >= ( TILES_Y * TILE_SIZE ) )
  {
    game->player.position.y = ( TILES_Y * TILE_SIZE ) - game->player.hitBoxSize.y - COLLISION_PADDING;
  }

  game->player.velocity.x = 0;
  game->player.velocity.y = 0;

  if ( prevPlayerPos.x != game->player.position.x || prevPlayerPos.y != game->player.position.y ||
       game->player.sprite.currentFrame != game->physics.spriteFrameCache )
  {
    cScreen_WipeSprite( &( game->screen ), &( game->tileMap ),
                        prevPlayerPos.x + game->player.spriteOffset.x,
                        prevPlayerPos.y + game->player.spriteOffset.y );
    game->physics.spriteFrameCache = game->player.sprite.currentFrame;
  }

  cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                      game->player.position.x + game->player.spriteOffset.x,
                      game->player.position.y + game->player.spriteOffset.y );
}
