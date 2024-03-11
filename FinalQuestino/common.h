#if !defined( COMMON_H )
#define COMMON_H

#include <Arduino.h>
#include <string.h>

// uncomment to use cSerial_PrintLn
//#define SERIAL_DEBUG

#define GAME_FPS                  30

#define TILE_SIZE                 16
#define PACKED_TILE_SIZE          8
#define TILES_X                   20
#define TILES_Y                   15
#define TILE_COUNT                300
#define TILE_TEXTURE_SIZE_BYTES   128 // 16x16 bit-packed, so 16/2 x 16
#define PORTAL_COUNT              3

#define SCREENS_X                 7
#define SCREENS_Y                 9

#define TILE_PASSABLE_FLAG        0x10
#define TILE_DAMAGE_FLAG          0x20
#define TILE_ENCOUNTERABLE_FLAG   0x40
#define TILE_HIGHRATE_FLAG        0x80

#define TEXT_TILES                44

#define SPRITE_SIZE               16
#define PACKED_SPRITE_SIZE        8
#define SPRITE_FRAMES             2

#define PLAYER_MAX_VELOCITY       64

#define COLLISION_PADDING 0.001f

typedef uint8_t cBool_t;
#define cTrue 1
#define cFalse 0
#define TOGGLE_BOOL( b ) b = b ? cFalse : cTrue

#ifdef __cplusplus
extern "C" {
#endif

#if defined( SERIAL_DEBUG )
void cSerial_PrintLn( const char* msg );
#endif

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
