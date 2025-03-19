#ifndef CREDITS_TEXT_H
#define CREDITS_TEXT_H
#include <stdint.h>

extern void set_credits_tile_row(uint8_t vram_row, uint8_t row) BANKED;
extern void set_credits_tiles_initial(void) BANKED;

#endif
