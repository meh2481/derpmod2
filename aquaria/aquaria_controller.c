#pragma bank 0

#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <gbdk/platform.h>
#include <gbdk/far_ptr.h>
#include <stdlib.h>
#include "../utils/hUGEHelpers.h"
#include "../utils/utils.h"
#include "tileset.h"
#include "map1_tiles.h"

extern void set_aquaria_map_tiles(void) BANKED;

void init_aquaria(void) NONBANKED {
  gamestate = STATE_PLAY;
  hUGE_init(&the_traveller);

  gb_decompress_bkg_data(0, tileset);

  // Set up the palette
  set_bkg_palette(0, 6, tilesetCGBPal);

  // SWITCH_ROM(2);
  set_aquaria_map_tiles();
  // SWITCH_ROM(0);
}

void update_aquaria(void) {
  // Do nothing for now
}
