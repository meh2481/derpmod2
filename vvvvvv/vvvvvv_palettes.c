#pragma bank 255

#include "vvvvvv_palettes.h"
#include <gb/gb.h>
#include <gb/cgb.h>

const palette_color_t* const vvvvvvCGBPal[] = {
  0,
  RGB(16,17,29),
  RGB(10,11,21),
  RGB(6,7,13),
  0,
  RGB(28,17,28),
  RGB(22,12,22),
  RGB(15,8,15),
  0,
  RGB(30,15,18),
  RGB(23,11,13),
  RGB(17,7,10),
  0,
  RGB(28,29,16),
  RGB(22,23,11),
  RGB(16,16,8),
  0,
  RGB(18,29,16),
  RGB(13,23,11),
  RGB(10,16,8),
  0,
  RGB(17,20,28),
  RGB(10,14,24),
  RGB(7,10,17),
  0,
  RGB(28,17,18),
  RGB(24,10,12),
  RGB(17,7,8),
  0,
  RGB(17,21,28),
  RGB(12,16,22),
  RGB(8,12,15),
  0,
  RGB(17,29,17),
  RGB(12,22,12),
  RGB(8,14,7),
  0,
  RGB(30,26,15),
  RGB(22,18,10),
  RGB(14,11,5),
  0,
  RGB(17,29,25),
  RGB(12,22,19),
  RGB(7,14,11),
  0,
  RGB(29,17,27),
  RGB(22,11,21),
  RGB(15,6,14),
  0,
  RGB(29,16,17),
  RGB(23,11,12),
  RGB(16,8,9),
};

#define NUM_PALETTES 13

void set_vvvvvv_room_palette(uint8_t pal) BANKED {
  set_bkg_palette(0, 1, &vvvvvvCGBPal[(pal % NUM_PALETTES) * 4]);
}
