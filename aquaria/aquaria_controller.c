#pragma bank 0

#include <gb/gb.h>
#include "../utils/hUGEHelpers.h"
#include "tileset.h"
#include "map1_tiles.h"

void init_aquaria(void) NONBANKED {
  init_aquaria_music();
  init_aquaria_tileset();
  set_aquaria_map_tiles();
  set_aquaria_map_tile_attribs();
}

void update_aquaria(void) NONBANKED {
  // Do nothing for now
}
