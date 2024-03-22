#if !defined( COMMON_H )
#define COMMON_H

#include <Arduino.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

// un-comment to use cSerial_PrintLn
//#define DEBUG_SERIAL
// un-comment to turn off encounters
//#define DEBUG_NOENCOUNTERS

#define GAME_FPS                    30

#define TILE_SIZE                   16
#define PACKED_TILE_SIZE            8
#define TILES_X                     20
#define TILES_Y                     15
#define TILE_COUNT                  300
#define TILE_TEXTURE_SIZE_BYTES     128 // 16x16 bit-packed, so 16/2 x 16
#define PORTAL_COUNT                4

#define TILE_FLAG_PASSABLE          0x10
#define TILE_FLAG_DAMAGE            0x20
#define TILE_FLAG_ENCOUNTERABLE     0x40
#define TILE_FLAG_HIGHENCOUNTERRATE 0x80

#define TEXT_TILES                  73

#define SPRITE_SIZE                 16
#define PACKED_SPRITE_SIZE          8
#define SPRITE_FRAMES               2

#define PLAYER_VELOCITY_MAX         48

#define MENU_CARAT_BLINKRATE        0.3f

#define COLLISION_PADDING           0.001f

#define ENCOUNTER_RATE_NORMAL       4
#define ENCOUNTER_RATE_HIGH         7

typedef uint8_t cBool_t;
#define cTrue 1
#define cFalse 0
#define TOGGLE_BOOL( b ) b = b ? cFalse : cTrue

#ifdef __cplusplus
extern "C" {
#endif

#if defined( DEBUG_SERIAL )
void cSerial_PrintLn( const char* msg );
#endif

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
