#include "game.h"

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW 200
#define ANALOG_THRESHOLD_HIGH 824

internal void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down );
internal void Input_HandleMapStateInput( Game_t* game );
internal void Input_HandleMenuStateInput( Game_t* game );
internal Bool_t Input_AnyButtonPressed( Input_t* input );

void Input_Init( Input_t* input )
{
   uint8_t i;

   for ( i = 0; i < BUTTON_COUNT; i++ )
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
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( input );
#else
   int16_t xValue = analogRead( PIN_ANALOG_X );
   int16_t yValue = analogRead( PIN_ANALOG_Y );

   Input_UpdateButtonState( &( input->buttonStates[BUTTON_LEFT] ), xValue > ANALOG_THRESHOLD_HIGH );
   Input_UpdateButtonState( &( input->buttonStates[BUTTON_UP] ), yValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[BUTTON_RIGHT] ), xValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[BUTTON_DOWN] ), yValue > ANALOG_THRESHOLD_HIGH );

   Input_UpdateButtonState( &( input->buttonStates[BUTTON_A] ), digitalRead( PIN_A_BUTTON ) == LOW );
   Input_UpdateButtonState( &( input->buttonStates[BUTTON_B] ), digitalRead( PIN_B_BUTTON ) == LOW );
#endif
}

#if defined VISUAL_STUDIO_DEV

void Input_ResetState( Input_t* input )
{
   uint32_t i;
   ButtonState_t* state = input->buttonStates;

   for ( i = 0; i < BUTTON_COUNT; i++ )
   {
      state->pressed = False;
      state->released = False;
      state++;
   }
}

void Input_ButtonPressed( Input_t* input, uint8_t button )
{
   ButtonState_t* state = &( input->buttonStates[button] );

   if ( !state->down )
   {
      state->down = True;
      state->pressed = True;
   }
}

void Input_ButtonReleased( Input_t* input, uint8_t button )
{
   ButtonState_t* state = &( input->buttonStates[button] );

   if ( state->down )
   {
      state->down = False;
      state->released = True;
   }
}

#endif // VISUAL_STUDIO_DEV

internal void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down )
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
      case GAMESTATE_MAP:
         Input_HandleMapStateInput( game );
         break;
      case GAMESTATE_MAPMENU:
      case GAMESTATE_BATTLEMENUMAIN:
         Input_HandleMenuStateInput( game );
         break;
      case GAMESTATE_MAPMESSAGE:
         if ( Input_AnyButtonPressed( &( game->input ) ) )
         {
            Game_WipeMessage( game );
            Screen_DrawActors( game );
            game->state = GAMESTATE_MAP;
         }
         break;
      case GAMESTATE_MAPSTATUS:
         if ( Input_AnyButtonPressed( &( game->input ) ) )
         {
            Game_WipeMapStatus( game );
            Screen_DrawActors( game );
            game->state = GAMESTATE_MAP;
         }
         break;
      case GAMESTATE_BATTLEFLEEATTEMPT:
         if ( Input_AnyButtonPressed( &( game->input ) ) )
         {
            Battle_ExecuteFlee( game );
         }
         break;
      case GAMESTATE_BATTLERESULT:
         if ( Input_AnyButtonPressed( &( game->input ) ) )
         {
            Battle_Collect( game );
         }
         break;
      case GAMESTATE_BATTLECOLLECT:
         if ( Input_AnyButtonPressed( &( game->input ) ) )
         {
            Battle_Done( game );
         }
         break;
   }
}

internal void Input_HandleMapStateInput( Game_t* game )
{
   Player_t* player = &( game->player );
   Sprite_t* sprite = &( player->sprite );
   Bool_t leftIsDown, upIsDown, rightIsDown, downIsDown;

   if ( game->input.buttonStates[BUTTON_A].pressed )
   {
      Game_ShowMapQuickStats( game );
      Menu_Load( &( game->menu ), MENUINDEX_MAP );
      Menu_Draw( game );
      game->state = GAMESTATE_MAPMENU;
   }
   else
   {
      leftIsDown = game->input.buttonStates[BUTTON_LEFT].down;
      upIsDown = game->input.buttonStates[BUTTON_UP].down;
      rightIsDown = game->input.buttonStates[BUTTON_RIGHT].down;
      downIsDown = game->input.buttonStates[BUTTON_DOWN].down;

      if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
      {
         if ( leftIsDown && !rightIsDown )
         {
            player->velocity.x = -( player->maxVelocity );

            if ( !( upIsDown && sprite->direction == DIRECTION_UP ) &&
                 !( downIsDown && sprite->direction == DIRECTION_DOWN ) )
            {
               sprite->direction = DIRECTION_LEFT;
            }

            if ( upIsDown || downIsDown )
            {
               player->velocity.x *= 0.707f;
            }
         }
         else if ( rightIsDown && !leftIsDown )
         {
            player->velocity.x = player->maxVelocity;

            if ( !( upIsDown && sprite->direction == DIRECTION_UP ) &&
                 !( downIsDown && sprite->direction == DIRECTION_DOWN ) )
            {
               sprite->direction = DIRECTION_RIGHT;
            }

            if ( upIsDown || downIsDown )
            {
               player->velocity.x *= 0.707f;
            }
         }

         if ( upIsDown && !downIsDown )
         {
            player->velocity.y = -( player->maxVelocity );

            if ( !( leftIsDown && sprite->direction == DIRECTION_LEFT ) &&
                 !( rightIsDown && sprite->direction == DIRECTION_RIGHT ) )
            {
               sprite->direction = DIRECTION_UP;
            }

            if ( leftIsDown || rightIsDown )
            {
               player->velocity.y *= 0.707f;
            }
         }
         else if ( downIsDown && !upIsDown )
         {
            player->velocity.y = player->maxVelocity;

            if ( !( leftIsDown && sprite->direction == DIRECTION_LEFT ) &&
                 !( rightIsDown && sprite->direction == DIRECTION_RIGHT ) )
            {
               sprite->direction = DIRECTION_DOWN;
            }

            if ( leftIsDown || rightIsDown )
            {
               player->velocity.y *= 0.707f;
            }
         }

         Sprite_Tic( &( player->sprite ) );
      }
   }
}

internal void Input_HandleMenuStateInput( Game_t* game )
{
   Bool_t upIsDown, downIsDown;

   if ( game->input.buttonStates[BUTTON_A].pressed )
   {
      Menu_Select( game );
   }
   else if ( game->input.buttonStates[BUTTON_B].pressed )
   {
      if ( game->state == GAMESTATE_MAPMENU )
      {
         Game_WipeMapQuickStats( game );
         Menu_Wipe( game );
         Screen_DrawActors( game );
         game->state = GAMESTATE_MAP;
      }
   }
   else
   {
      upIsDown = game->input.buttonStates[BUTTON_UP].pressed;
      downIsDown = game->input.buttonStates[BUTTON_DOWN].pressed;

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

internal Bool_t Input_AnyButtonPressed( Input_t* input )
{
   uint8_t i;

   for ( i = 0; i < BUTTON_COUNT; i++ )
   {
      if ( input->buttonStates[i].pressed )
      {
         return True;
      }
   }

   return False;
}
