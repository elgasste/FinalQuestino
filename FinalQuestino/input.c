#include "game.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

static void cInput_UpdateButtonState( cButtonState_t* buttonState, cBool_t down );
static void cInput_HandleMapStateInput( cGame_t* game );
static void cInput_HandleMapMenuStateInput( cGame_t* game );

void cInput_Init( cInput_t* input )
{
   uint8_t i;

   for ( i = 0; i < (uint8_t)cButton_Count; i++ )
   {
      input->buttonStates[i].pressed = cFalse;
      input->buttonStates[i].released = cFalse;
      input->buttonStates[i].down = cFalse;
   }

   pinMode( PIN_A_BUTTON, INPUT_PULLUP );
   pinMode( PIN_B_BUTTON, INPUT_PULLUP );
}

void cInput_Read( cInput_t* input )
{
   int16_t xValue = analogRead( PIN_ANALOG_X );
   int16_t yValue = analogRead( PIN_ANALOG_Y );

   cInput_UpdateButtonState( &( input->buttonStates[cButton_Left ] ), xValue > ANALOG_THRESHOLD_HIGH );
   cInput_UpdateButtonState( &( input->buttonStates[cButton_Up ] ), yValue < ANALOG_THRESHOLD_LOW );
   cInput_UpdateButtonState( &( input->buttonStates[cButton_Right ] ), xValue < ANALOG_THRESHOLD_LOW );
   cInput_UpdateButtonState( &( input->buttonStates[cButton_Down ] ), yValue > ANALOG_THRESHOLD_HIGH );

   cInput_UpdateButtonState( &( input->buttonStates[cButton_A] ), digitalRead( PIN_A_BUTTON ) == LOW );
   cInput_UpdateButtonState( &( input->buttonStates[cButton_B] ), digitalRead( PIN_B_BUTTON ) == LOW );
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
   switch( game->state )
   {
      case cGameState_Map:
         cInput_HandleMapStateInput( game );
         break;
      case cGameState_MapMenu:
         cInput_HandleMapMenuStateInput( game );
         break;
   }
}

static void cInput_HandleMapStateInput( cGame_t* game )
{
   cPlayer_t* player = &( game->player );
   cSprite_t* sprite = &( player->sprite );
   cBool_t leftIsDown, upIsDown, rightIsDown, downIsDown;

   if ( game->input.buttonStates[cButton_A].pressed )
   {
      cGame_ChangeState( game, cGameState_MapMenu );
   }
   else
   {
      leftIsDown = game->input.buttonStates[cButton_Left].down;
      upIsDown = game->input.buttonStates[cButton_Up].down;
      rightIsDown = game->input.buttonStates[cButton_Right].down;
      downIsDown = game->input.buttonStates[cButton_Down].down;

      if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
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

         cSprite_Tic( &( player->sprite ), &( game->clock ) );
      }
   }
}

static void cInput_HandleMapMenuStateInput( cGame_t* game )
{
   cBool_t upIsDown, downIsDown;

   if ( game->input.buttonStates[cButton_A].pressed )
   {
      cMenu_Select( game );
   }
   else if ( game->input.buttonStates[cButton_B].pressed )
   {
      cGame_ChangeState( game, cGameState_Map );
   }
   else
   {
      upIsDown = game->input.buttonStates[cButton_Up].pressed;
      downIsDown = game->input.buttonStates[cButton_Down].pressed;

      if ( upIsDown && !downIsDown )
      {
         cMenu_ScrollUp( game );
      }
      else if ( downIsDown && !upIsDown )
      {
         cMenu_ScrollDown( game );
      }
   }
}
