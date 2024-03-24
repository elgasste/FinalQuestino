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

#define GAME_FPS                          30

#define MAP_TILE_SIZE                     16
#define MAP_PACKED_TILE_SIZE              8
#define MAP_TILES_X                       20
#define MAP_TILES_Y                       15
#define MAP_TILE_COUNT                    300
#define MAP_TILE_TEXTURE_SIZE_BYTES       128 // 16x16 bit-packed, so 16/2 * 16
#define MAP_PORTAL_COUNT                  4

#define MAP_TILE_FLAG_PASSABLE            0x10
#define MAP_TILE_FLAG_DAMAGE              0x20
#define MAP_TILE_FLAG_ENCOUNTERABLE       0x40
#define MAP_TILE_FLAG_HIGHENCOUNTERRATE   0x80

#define TEXT_TILE_COUNT                   73

#define SPRITE_SIZE                       16
#define PACKED_SPRITE_SIZE                8
#define SPRITE_FRAMES                     2

#define ENEMY_TILE_TEXTURE_COUNT          80
#define ENEMY_TILE_TEXTURE_SIZE_BYTES     32 // 8x8 bit-packed, so 8/2 * 8
#define ENEMY_TILES_X                     10
#define ENEMY_TILES_Y                     12
#define ENEMY_TILE_COUNT                  120

#define PLAYER_VELOCITY_MAX               48

#define MENU_CARAT_BLINKRATE              0.3f

#define COLLISION_PADDING                 0.001f

#define ENCOUNTER_RATE_NORMAL             4
#define ENCOUNTER_RATE_HIGH               7

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
