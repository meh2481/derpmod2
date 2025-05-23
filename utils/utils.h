#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>

// Global defines
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH_TILES 20
#define SCREEN_HEIGHT_TILES 18

#define WIN_X_OFFSET 7

#define STATE_START  0
#define STATE_PLAY   1
#define STATE_INTRO  2
#define STATE_LOGO   3
#define STATE_VVVVVV 4

// Globals
extern uint8_t i, j, tmp, tmp_counter;
extern uint8_t gamestate;

// TODO: Comment out on release
// #define DEBUG

#endif
