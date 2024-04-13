#if !defined( COMMON_H )
#define COMMON_H

#include <Arduino.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

// un-comment to use cSerial_PrintLn
//#define DEBUG_SERIAL
// un-comment to turn off encounters when the B button is pressed
//#define DEBUG_NOENCOUNTERSONB
// un-comment to turn on fast walk mode
//#define DEBUG_FASTWALK

#define GAME_FPS                          30
#define FRAME_MICROSECONDS                33333
#define FRAME_SECONDS                     0.03333333f

#define MAP_TILE_SIZE                     16
#define MAP_PACKED_TILE_SIZE              8
#define MAP_TILES_X                       20
#define MAP_TILES_Y                       15
#define MAP_TILE_COUNT                    300
#define MAP_TILE_TEXTURE_COUNT            19
#define MAP_TILE_TEXTURE_SIZE_BYTES       128 // 16x16 bit-packed, so 16/2 * 16
#define MAP_PORTAL_COUNT                  4
#define MAP_TILE_ENEMY_INDEX_COUNT        4

#define MAP_TILE_FLAG_PASSABLE            0x20
#define MAP_TILE_FLAG_ENCOUNTERABLE       0x40

#define TEXT_TILE_COUNT                   73

#define SPRITE_SIZE                       16
#define PACKED_SPRITE_SIZE                8
#define SPRITE_FRAMES                     2

#define ENEMY_TILE_TEXTURE_COUNT          78
#define ENEMY_TILE_TEXTURE_SIZE_BYTES     32 // 8x8 bit-packed, so 8/2 * 8
#define ENEMY_TILE_SIZE                   8
#define ENEMY_TILES_X                     10
#define ENEMY_TILES_Y                     12
#define ENEMY_TILE_COUNT                  120

#define SPECIALENEMYID_GREENDRAGON        35
#define SPECIALENEMYID_GOLEM              27
#define SPECIALENEMYID_AXEKNIGHT          33

#define SPECIALENEMYMAP_GREENDRAGON       64
#define SPECIALENEMYMAP_GOLEM             53
#define SPECIALENEMYMAP_AXEKNIGHT         70

#define SPECIALENEMYTILE_GREENDRAGON      190
#define SPECIALENEMYTILE_GOLEM            44
#define SPECIALENEMYTILE_AXEKNIGHT        48

#define SPECIALENEMYFLAG_GREENDRAGON      0x1
#define SPECIALENEMYFLAG_GOLEM            0x2
#define SPECIALENEMYFLAG_AXEKNIGHT        0x4

#if defined( DEBUG_FASTWALK )
#define PLAYERVELOCITY_NORMAL             96
#define PLAYERVELOCITY_SLOW               96
#define PLAYERVELOCITY_SLOWER             96
#define PLAYERVELOCITY_CRAWL              96
#else
#define PLAYERVELOCITY_NORMAL             64
#define PLAYERVELOCITY_SLOW               56
#define PLAYERVELOCITY_SLOWER             40
#define PLAYERVELOCITY_CRAWL              24
#endif
#define PLAYER_HITBOX_SIZE                12
#define PLAYER_SPRITEOFFSET_X             -2
#define PLAYER_SPRITEOFFSET_Y             -4

#define MENU_CARAT_BLINKRATE              0.3f

#define COLLISION_PADDING                 0.001f

#define ENCOUNTERRATE_LOW                4
#define ENCOUNTERRATE_MEDIUM             6
#define ENCOUNTERRATE_HIGH               8

typedef uint8_t cBool_t;
#define cTrue 1
#define cFalse 0
#define TOGGLE_BOOL( b ) b = b ? cFalse : cTrue

#if defined( DEBUG_SERIAL )

#if defined( __cplusplus )
extern "C" {
#endif

void cSerial_PrintLn( const char* msg );

#if defined( __cplusplus )
}
#endif

#endif // DEBUG_SERIAL

#endif // COMMON_H
