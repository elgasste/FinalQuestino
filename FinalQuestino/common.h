#if !defined( COMMON_H )
#define COMMON_H

#if !defined( VISUAL_STUDIO_DEV )
#include <Arduino.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

// un-comment to use Serial_PrintLn
//#define DEBUG_SERIAL
// un-comment to turn off encounters when the B button is pressed
//#define DEBUG_NOENCOUNTERSONB
// un-comment to turn on fast walk mode
//#define DEBUG_FASTWALK

// not really necessary, but makes things easier to distinguish
#define internal static
#define global static
#define local_persist static

#define UNUSED_PARAM( x )                    (void)x

#if defined( VISUAL_STUDIO_DEV )
#define DELAY_MS( x )                        Sleep( x )
#else
#define DELAY_MS( x )                        delay( x )
#define MICROS( x )                          micros( x )
#endif

#define MAX_I( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define MIN_I( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

#if defined( VISUAL_STUDIO_DEV )
#define GAME_FPS                             60
#define FRAME_MICROSECONDS                   16666
#define FRAME_SECONDS                        0.01666666f
#else
#define GAME_FPS                             30
#define FRAME_MICROSECONDS                   33333
#define FRAME_SECONDS                        0.03333333f
#endif

#define DIRECTION_LEFT                       0
#define DIRECTION_UP                         1
#define DIRECTION_RIGHT                      2
#define DIRECTION_DOWN                       3

#define GAMESTATE_READY                      0
#define GAMESTATE_MAP                        1
#define GAMESTATE_MAPMENU                    2
#define GAMESTATE_MAPMESSAGE                 3
#define GAMESTATE_MAPSTATUS                  4
#define GAMESTATE_BATTLEMENUMAIN             5
#define GAMESTATE_BATTLERESULT               6

#define BUTTON_LEFT                          0
#define BUTTON_UP                            1
#define BUTTON_RIGHT                         2
#define BUTTON_DOWN                          3
#define BUTTON_A                             4
#define BUTTON_B                             5
#define BUTTON_COUNT                         6

#define MENUINDEX_MAP                        0
#define MENUINDEX_BATTLEMAIN                 1

#define INDEFINITEARTICLE_A                  0
#define INDEFINITEARTICLE_AN                 1

#define MAP_TILE_SIZE                        16
#define MAP_PACKED_TILE_SIZE                 8
#define MAP_TILES_X                          20
#define MAP_TILES_Y                          15
#define MAP_TILE_COUNT                       300
#define MAP_TILE_TEXTURE_COUNT               16
#define MAP_TILE_TEXTURE_SIZE_BYTES          128 // 16x16 bit-packed, so 16/2 * 16
#define MAP_PORTAL_COUNT                     6
#define MAP_TILE_ENEMY_INDEX_COUNT           4
#define MAP_SPRITE_COUNT                     16

#define GET_TILE_TEXTURE_INDEX( x )          ( ( x ) & 0x1F )
#define GET_TILE_SPEED( x )                  ( ( ( x ) & 0xC ) >> 2 )
#define GET_SPRITE_TILE_INDEX( x )           ( ( x ) & 0x1FF )
#define GET_PORTAL_ORIGIN( x )               ( ( x ) >> 21 )
#define GET_PORTAL_DEST_TILE_MAP_INDEX( x )  ( ( ( x ) & 0x1FF800 ) >> 11 )
#define GET_PORTAL_DEST_TILE_INDEX( x )      ( ( x ) & 0x7FF )
#define GET_ENCOUNTER_RATE( x )              ( ( x ) & 0x3 )
#define GET_TILE_PASSABLE( x )               ( ( ( x ) & 0x20 ) >> 5 )
#define GET_TILE_ENCOUNTERABLE( x )          ( ( ( x ) & 0x40 ) >> 6 )
#define GET_SPRITE_PASSABLE( x )             ( ( ( x ) & 0x2000 ) >> 13 )
#define GET_SPRITE_INDEX( x )                ( ( ( x ) & 0x1E00 ) >> 9 )

#define TEXT_TILE_COUNT                      73

#define SPRITE_SIZE                          16
#define SPRITE_PACKED_SIZE                   8
#define SPRITE_FRAMES                        2
#define SPRITE_TEXTURE_SIZE_BYTES            128 // 16x16 bit-packed, so 16/2 * 16
#define SPRITE_FRAME_SECONDS                 0.2f

#define ENEMY_TILE_TEXTURE_COUNT             78
#define ENEMY_TILE_TEXTURE_SIZE_BYTES        32 // 8x8 bit-packed, so 8/2 * 8
#define ENEMY_TILE_SIZE                      8
#define ENEMY_TILES_X                        10
#define ENEMY_TILES_Y                        12
#define ENEMY_TILE_COUNT                     120

#define SPECIALENEMYID_GREENDRAGON           35
#define SPECIALENEMYID_GOLEM                 27
#define SPECIALENEMYID_AXEKNIGHT             33

#define SPECIALENEMYMAP_GREENDRAGON          64
#define SPECIALENEMYMAP_GOLEM                53
#define SPECIALENEMYMAP_AXEKNIGHT            70

#define SPECIALENEMYTILE_GREENDRAGON         190
#define SPECIALENEMYTILE_GOLEM               44
#define SPECIALENEMYTILE_AXEKNIGHT           48

#define SPECIALENEMYFLAG_GREENDRAGON         0x1
#define SPECIALENEMYFLAG_GOLEM               0x2
#define SPECIALENEMYFLAG_AXEKNIGHT           0x4

#if defined( DEBUG_FASTWALK )
#define PLAYERVELOCITY_NORMAL                96
#define PLAYERVELOCITY_SLOW                  96
#define PLAYERVELOCITY_SLOWER                96
#define PLAYERVELOCITY_CRAWL                 96
#else
#define PLAYERVELOCITY_NORMAL                64
#define PLAYERVELOCITY_SLOW                  56
#define PLAYERVELOCITY_SLOWER                40
#define PLAYERVELOCITY_CRAWL                 24
#endif
#define PLAYER_HITBOX_SIZE                   12
#define PLAYER_SPRITEOFFSET_X                -2
#define PLAYER_SPRITEOFFSET_Y                -4

#define MENU_CARAT_BLINKRATE                 0.3f

#define COLLISION_PADDING                    0.001f

#define ENCOUNTERRATE_LOW                    4
#define ENCOUNTERRATE_MEDIUM                 6
#define ENCOUNTERRATE_HIGH                   8

typedef uint8_t Bool_t;
#define True 1
#define False 0
#define TOGGLE_BOOL( b ) b = b ? False : True

#if defined( DEBUG_SERIAL )

#if defined( __cplusplus )
extern "C" {
#endif

void Serial_PrintLn( const char* msg );

#if defined( __cplusplus )
}
#endif

#endif // DEBUG_SERIAL

#endif // COMMON_H
