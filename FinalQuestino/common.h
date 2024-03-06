#if !defined( COMMON_H )
#define COMMON_H

#include <Arduino.h>

#define GAME_FPS                  30

#define TILE_SIZE                 16
#define PACKED_TILE_SIZE          8
#define TILES_X                   20
#define TILES_Y                   15
#define TILE_COUNT                300
#define TILE_TEXTURE_SIZE_BYTES   128 // 16x16 bit-packed, so 16/2 x 16

#define TEXT_TILES                44

#define SPRITE_SIZE               16
#define SPRITE_FRAMES             2

#define PLAYER_MAX_VELOCITY       48

typedef uint8_t cBool_t;
#define cTrue 1
#define cFalse 0
#define TOGGLE_BOOL( b ) b = b ? cFalse : cTrue

#ifdef __cplusplus
extern "C" {
#endif

void cSerial_PrintLn( const char* msg );

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
