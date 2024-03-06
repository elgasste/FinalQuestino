#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "window.h"
#include "clock.h"
#include "player.h"

typedef struct cGame_t
{
  cWindow_t window;
  cClock_t clock;
  cGameState_t state;
  cPlayer_t player;
}
cGame_t;

#if defined( __cplusplus )
extern "C" {
#endif

void cGame_Init( cGame_t* game );
void cGame_Tic( cGame_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
