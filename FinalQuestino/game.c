#include "game.h"

#define COLLISION_PADDING 0.001f

static void cGame_HandleInput( cGame_t* game );

void cGame_Init( cGame_t* game )
{
  cScreen_Init( &( game->screen ) );
  cScreen_LoadPalette( &( game->screen ), 0 );
  cScreen_Begin( &( game->screen ) );

  cClock_Init( &( game->clock ) );
  cInputReader_Init( &( game->inputReader ) );

  cTileMap_Init( &( game->tileMap ) );
  cTileMap_LoadMap( &( game->tileMap ), 0 );
  cGame_LoadTextBitFields( game );

  cPlayer_Init( &( game->player ) );
  game->player.sprite.direction = cDirection_Down;
  game->player.sprite.frameSeconds = 0.2f;
  game->player.position.x = ( TILES_X * TILE_SIZE ) / 2;
  game->player.position.y = ( TILES_Y * TILE_SIZE ) / 2;
  game->player.hitBoxSize.x = 12;
  game->player.hitBoxSize.y = 12;
  game->player.spriteOffset.x = -2;
  game->player.spriteOffset.y = -4;

  game->state = cGameState_Init;
}

void cGame_Tic( cGame_t* game )
{
  cVector2f_t prevPlayerPos;

  cInputReader_ReadInput( &( game->inputReader ) );
  cGame_HandleInput( game );

  switch( game->state )
  {
    case cGameState_Init:
      cScreen_DrawTileMap( &( game->screen ), &( game->tileMap ) );
      game->state = cGameState_Playing;
      break;
    case cGameState_Playing:
      // TODO: this should go into a physics file, probably
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
      if ( prevPlayerPos.x != game->player.position.x || prevPlayerPos.y != game->player.position.y )
      {
        // TODO: this should happen on a new sprite frame as well
        cScreen_WipeSprite( &( game->screen ), &( game->tileMap ),
                            prevPlayerPos.x + game->player.spriteOffset.x,
                            prevPlayerPos.y + game->player.spriteOffset.y );
      }
      cScreen_DrawSprite( &( game->screen ), &( game->player.sprite ), &( game->tileMap ),
                          game->player.position.x + game->player.spriteOffset.x,
                          game->player.position.y + game->player.spriteOffset.y );
      break;
  }
}

static void cGame_HandleInput( cGame_t* game )
{
  cPlayer_t* player = &( game->player );
  cSprite_t* sprite = &( player->sprite );
  cBool_t leftIsDown, upIsDown, rightIsDown, downIsDown;

  // TODO: this should go into an input handler file, probably
  if ( game->state == cGameState_Playing )
  {
    leftIsDown = game->inputReader.buttonStates[cButton_Left].down;
    upIsDown = game->inputReader.buttonStates[cButton_Up].down;
    rightIsDown = game->inputReader.buttonStates[cButton_Right].down;
    downIsDown = game->inputReader.buttonStates[cButton_Down].down;

    if ( game->state == cGameState_Playing )
    {
      if ( leftIsDown && !rightIsDown )
      {
        player->velocity.x = -( player->maxVelocity.x );

        if ( !( upIsDown && sprite->direction == cDirection_Up ) &&
            !( downIsDown && sprite->direction == cDirection_Down ) )
        {
          sprite->direction = cDirection_Left;
        }

        if ( upIsDown || downIsDown )
        {
          player->velocity.x *= 0.707;
        }
      }
      else if ( rightIsDown && !leftIsDown )
      {
        player->velocity.x = player->maxVelocity.x;

        if ( !( upIsDown && sprite->direction == cDirection_Up ) &&
            !( downIsDown && sprite->direction == cDirection_Down ) )
        {
          sprite->direction = cDirection_Right;
        }

        if ( upIsDown || downIsDown )
        {
          player->velocity.x *= 0.707;
        }
      }

      if ( upIsDown && !downIsDown )
      {
        player->velocity.y = -( player->maxVelocity.y );

        if ( !( leftIsDown && sprite->direction == cDirection_Left ) &&
            !( rightIsDown && sprite->direction == cDirection_Right ) )
        {
          sprite->direction = cDirection_Up;
        }

        if ( leftIsDown || rightIsDown )
        {
          player->velocity.y *= 0.707;
        }
      }
      else if ( downIsDown && !upIsDown )
      {
        player->velocity.y = player->maxVelocity.y;

        if ( !( leftIsDown && sprite->direction == cDirection_Left ) &&
            !( rightIsDown && sprite->direction == cDirection_Right ) )
        {
          sprite->direction = cDirection_Down;
        }

        if ( leftIsDown || rightIsDown )
        {
          player->velocity.y *= 0.707;
        }
      }
    }

    if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
    {
      cSprite_Tic( &( player->sprite ), &( game->clock ) );
    }
  }
}
