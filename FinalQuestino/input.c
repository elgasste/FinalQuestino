#include "game.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

static void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down );
static void Input_HandleMapStateInput( Game_t* game );
static void Input_HandleMapMenuStateInput( Game_t* game );

void Input_Init( Input_t* input )
{
   uint8_t i;

   for ( i = 0; i < (uint8_t)Button_Count; i++ )
   {
      input->buttonStates[i].pressed = False;
      input->buttonStates[i].released = False;
      input->buttonStates[i].down = False;
   }

#if !defined( VISUAL_STUDIO_DEV )
   pinMode( PIN_A_BUTTON, INPUT_PULLUP );
   pinMode( PIN_B_BUTTON, INPUT_PULLUP );
#endif
}

void Input_Read( Input_t* input )
{
#if !defined( VISUAL_STUDIO_DEV )
   int16_t xValue = analogRead( PIN_ANALOG_X );
   int16_t yValue = analogRead( PIN_ANALOG_Y );

   Input_UpdateButtonState( &( input->buttonStates[Button_Left ] ), xValue > ANALOG_THRESHOLD_HIGH );
   Input_UpdateButtonState( &( input->buttonStates[Button_Up ] ), yValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_Right ] ), xValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_Down ] ), yValue > ANALOG_THRESHOLD_HIGH );

   Input_UpdateButtonState( &( input->buttonStates[Button_A] ), digitalRead( PIN_A_BUTTON ) == LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_B] ), digitalRead( PIN_B_BUTTON ) == LOW );
#endif
}

#if defined VISUAL_STUDIO_DEV

void Input_ResetState( Input_t* input )
{
   uint32_t i;
   ButtonState_t* state = input->buttonStates;

   for ( i = 0; i < (uint32_t)Button_Count; i++ )
   {
      state->pressed = False;
      state->released = False;
      state++;
   }
}

void Input_ButtonPressed( Input_t* input, Button_t button )
{
   ButtonState_t* state = &( input->buttonStates[button] );

   if ( !state->down )
   {
      state->down = True;
      state->pressed = True;
   }
}

void Input_ButtonReleased( Input_t* input, Button_t button )
{
   ButtonState_t* state = &( input->buttonStates[button] );

   if ( state->down )
   {
      state->down = False;
      state->released = True;
   }
}

#endif // VISUAL_STUDIO_DEV

static void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down )
{
   if ( down )
   {
      buttonState->released = False;
      buttonState->pressed = buttonState->down ? False : True;
   }
   else
   {
      buttonState->pressed = False;
      buttonState->released = buttonState->down ? True : False;
   }

   buttonState->down = down;
}

void Input_Handle( Game_t* game )
{
   switch( game->state )
   {
      case GameState_Map:
         Input_HandleMapStateInput( game );
         break;
      case GameState_MapMenu:
         Input_HandleMapMenuStateInput( game );
         break;
      case GameState_MapMessage:
      case GameState_MapStatus:
      case GameState_Battle:
         if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
         {
            Game_ChangeState( game, GameState_Map );
         }
         break;
   }
}

static void Input_HandleMapStateInput( Game_t* game )
{
   Player_t* player = &( game->player );
   Sprite_t* sprite = &( player->sprite );
   Bool_t leftIsDown, upIsDown, rightIsDown, downIsDown;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Game_ChangeState( game, GameState_MapMenu );
   }
   else
   {
      leftIsDown = game->input.buttonStates[Button_Left].down;
      upIsDown = game->input.buttonStates[Button_Up].down;
      rightIsDown = game->input.buttonStates[Button_Right].down;
      downIsDown = game->input.buttonStates[Button_Down].down;

      if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
      {
         if ( leftIsDown && !rightIsDown )
         {
            player->velocity.x = -( player->maxVelocity );

            if ( !( upIsDown && sprite->direction == Direction_Up ) &&
                 !( downIsDown && sprite->direction == Direction_Down ) )
            {
               sprite->direction = Direction_Left;
            }

            if ( upIsDown || downIsDown )
            {
               player->velocity.x *= 0.707f;
            }
         }
         else if ( rightIsDown && !leftIsDown )
         {
            player->velocity.x = player->maxVelocity;

            if ( !( upIsDown && sprite->direction == Direction_Up ) &&
                 !( downIsDown && sprite->direction == Direction_Down ) )
            {
               sprite->direction = Direction_Right;
            }

            if ( upIsDown || downIsDown )
            {
               player->velocity.x *= 0.707f;
            }
         }

         if ( upIsDown && !downIsDown )
         {
            player->velocity.y = -( player->maxVelocity );

            if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
                 !( rightIsDown && sprite->direction == Direction_Right ) )
            {
               sprite->direction = Direction_Up;
            }

            if ( leftIsDown || rightIsDown )
            {
               player->velocity.y *= 0.707f;
            }
         }
         else if ( downIsDown && !upIsDown )
         {
            player->velocity.y = player->maxVelocity;

            if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
                 !( rightIsDown && sprite->direction == Direction_Right ) )
            {
               sprite->direction = Direction_Down;
            }

            if ( leftIsDown || rightIsDown )
            {
               player->velocity.y *= 0.707f;
            }
         }

         Sprite_Tic( &( player->sprite ), &( game->clock ) );
      }
   }
}

static void Input_HandleMapMenuStateInput( Game_t* game )
{
   Bool_t upIsDown, downIsDown;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_Select( game );
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      Game_ChangeState( game, GameState_Map );
   }
   else
   {
      upIsDown = game->input.buttonStates[Button_Up].pressed;
      downIsDown = game->input.buttonStates[Button_Down].pressed;

      if ( upIsDown && !downIsDown )
      {
         Menu_ScrollUp( game );
      }
      else if ( downIsDown && !upIsDown )
      {
         Menu_ScrollDown( game );
      }
   }
}
