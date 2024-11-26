#include <gb/gb.h>
#include "../utils/hUGEHelpers.h"
#include "tileset.h"
#include "map1_tiles.h"
#include "../utils/utils.h"

#define WIN_X_OFFSET 7

void init_aquaria(void) NONBANKED {
  gamestate = STATE_PLAY;
  init_aquaria_music();
  set_aquaria_map_tiles();
  set_aquaria_map_tile_attribs();

  init_aquaria_tileset();
  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
}

void update_aquaria(void) NONBANKED {
  // Do nothing for now
}
