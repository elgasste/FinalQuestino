#include "game.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

static void cInput_UpdateButtonState( cButtonState_t* buttonState, cBool_t down );

void cInput_Init( cInput_t* input )
{
  uint8_t i;

  for( i = 0; i < (uint8_t)cButton_Count; i++ )
  {
    input->buttonStates[i].pressed = cFalse;
    input->buttonStates[i].released = cFalse;
    input->buttonStates[i].down = cFalse;
  }
}

void cInput_Read( cInput_t* input )
{
  int xValue = analogRead( PIN_ANALOG_X );
  int yValue = analogRead( PIN_ANALOG_Y );

  cInput_UpdateButtonState( &( input->buttonStates[cButton_Left ] ), xValue > ANALOG_THRESHOLD_HIGH );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Up ] ), yValue < ANALOG_THRESHOLD_LOW );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Right ] ), xValue < ANALOG_THRESHOLD_LOW );
  cInput_UpdateButtonState( &( input->buttonStates[cButton_Down ] ), yValue > ANALOG_THRESHOLD_HIGH );
}

static void cInput_UpdateButtonState( cButtonState_t* buttonState, cBool_t down )
{
  if ( down )
  {
    buttonState->released = cFalse;
    buttonState->pressed = buttonState->down ? cFalse : cTrue;
  }
  else
  {
    buttonState->pressed = cFalse;
    buttonState->released = buttonState->down ? cTrue : cFalse;
  }

  buttonState->down = down;
}

void cInput_Handle( cGame_t* game )
{
  cPlayer_t* player = &( game->player );
  cSprite_t* sprite = &( player->sprite );
  cBool_t leftIsDown, upIsDown, rightIsDown, downIsDown;

  if ( game->state == cGameState_Playing )
  {
    leftIsDown = game->input.buttonStates[cButton_Left].down;
    upIsDown = game->input.buttonStates[cButton_Up].down;
    rightIsDown = game->input.buttonStates[cButton_Right].down;
    downIsDown = game->input.buttonStates[cButton_Down].down;

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
