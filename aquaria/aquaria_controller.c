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

uint16_t bg_pos_x = 0;
uint16_t bg_pos_y = 0;

uint16_t prev_bg_pos_x = 0;
uint16_t prev_bg_pos_y = 0;

void update_aquaria(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(aquaria_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  prev_bg_pos_x = bg_pos_x;
  prev_bg_pos_y = bg_pos_y;

  if (input & J_LEFT) {
    bg_pos_x--;
  }
  if (input & J_RIGHT) {
    bg_pos_x++;
  }
  if (input & J_UP) {
    bg_pos_y--;
  }
  if (input & J_DOWN) {
    bg_pos_y++;
  }

  move_bkg(bg_pos_x & 0xFF, bg_pos_y & 0xFF);

  uint8_t prev_col = prev_bg_pos_x >> 3;
  uint8_t prev_row = prev_bg_pos_y >> 3;
  uint8_t cur_col = bg_pos_x >> 3;
  uint8_t cur_row = bg_pos_y >> 3;

  if (cur_col > prev_col) {
    set_aquaria_map_tile_column(cur_col + 20, cur_row, (cur_col + 20) % 32);
    set_aquaria_map_attrib_column(cur_col + 20, cur_row, (cur_col + 20) % 32);
  } else if (cur_col < prev_col) {
    set_aquaria_map_tile_column(cur_col, cur_row, cur_col % 32);
    set_aquaria_map_attrib_column(cur_col, cur_row, cur_col % 32);
  }

  if (cur_row > prev_row) {
    set_aquaria_map_tile_row(cur_row + 18, (cur_row + 18) % 32, cur_col);
    set_aquaria_map_attrib_row(cur_row + 18, (cur_row + 18) % 32, cur_col);
  } else if (cur_row < prev_row) {
    set_aquaria_map_tile_row(cur_row, cur_row % 32, cur_col);
    set_aquaria_map_attrib_row(cur_row, cur_row % 32, cur_col);
  }


}
