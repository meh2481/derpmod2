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
};

#define NUM_PALETTES 3

void set_vvvvvv_room_palette(uint8_t pal) BANKED {
  set_bkg_palette(0, 1, &vvvvvvCGBPal[(pal % NUM_PALETTES) * 4]);
}
