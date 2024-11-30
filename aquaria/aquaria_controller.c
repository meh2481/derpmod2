#include <gb/gb.h>
#include "../utils/hUGEHelpers.h"
#include "tileset.h"
#include "map1_tiles.h"
#include "../utils/utils.h"
#include "../sprite/naija_sprites.h"

#define WIN_X_OFFSET                7
#define PLAYER_SPRITE               0
#define DEFAULT_PLAYER_SPRITE_PROP  0x00

uint16_t bg_pos_x = 0;
uint16_t bg_pos_y = 0;
uint8_t direction = 0;
uint8_t last_direction = 0;
uint8_t last_pressed_horiz = 0;
#define DIRECTION_LEFT  1
#define DIRECTION_UP    2
#define DIRECTION_RIGHT 4
#define DIRECTION_DOWN  8

uint16_t prev_bg_pos_x = 0;
uint16_t prev_bg_pos_y = 0;

uint8_t prev_col, prev_row, cur_col, cur_row;

void init_aquaria(void) NONBANKED {
  gamestate = STATE_PLAY;
  init_aquaria_music();
  set_aquaria_map_tiles();
  set_aquaria_map_tile_attribs();

  // Setup sprite data
  SPRITES_8x8;
  set_fishform_sprite_tiles(0);
  set_fishform_sprite_palettes(0);

  // Create player sprite
  set_sprite_tile(PLAYER_SPRITE, 0);
  set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);

  // Create song note sprites
  tmp = 0x1; // Current sprite palette
  tmp_counter = 0;
  for(i = 0; i < 8; i++) {
    set_sprite_tile(i + 1, i + 4);
    set_sprite_prop(i + 1, tmp);
    tmp_counter++;
    // Every 3 sprites, change palette
    if (tmp_counter == 3) {
      tmp++;
      tmp_counter = 0;
    }
  }

  // Center sprite on the screen
  move_sprite(0, 84, 84);

  init_aquaria_tileset();
  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
}

void update_player_sprite(void) NONBANKED {
  if (direction == DIRECTION_LEFT) {
    set_sprite_tile(PLAYER_SPRITE, 0);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == DIRECTION_RIGHT) {
    set_sprite_tile(PLAYER_SPRITE, 0);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  } else if (direction == DIRECTION_UP) {
    set_sprite_tile(PLAYER_SPRITE, 1);
    if(last_pressed_horiz & DIRECTION_RIGHT) {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
    } else {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
    }
  } else if (direction == DIRECTION_DOWN) {
    set_sprite_tile(PLAYER_SPRITE, 1);
    if (last_pressed_horiz & DIRECTION_LEFT) {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX | S_FLIPY);
    } else {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPY);
    }
  } else if (direction == (DIRECTION_LEFT | DIRECTION_UP)) {
    set_sprite_tile(PLAYER_SPRITE, 2);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == (DIRECTION_LEFT | DIRECTION_DOWN)) {
    set_sprite_tile(PLAYER_SPRITE, 3);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == (DIRECTION_RIGHT | DIRECTION_UP)) {
    set_sprite_tile(PLAYER_SPRITE, 2);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  } else if (direction == (DIRECTION_RIGHT | DIRECTION_DOWN)) {
    set_sprite_tile(PLAYER_SPRITE, 3);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  }
}

void update_aquaria(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(aquaria_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  prev_bg_pos_x = bg_pos_x;
  prev_bg_pos_y = bg_pos_y;

  last_direction = direction;
  direction = 0;

  if (input & J_LEFT) {
    bg_pos_x--;
    direction |= DIRECTION_LEFT;
    last_pressed_horiz = DIRECTION_LEFT;
  }
  if (input & J_RIGHT) {
    bg_pos_x++;
    direction |= DIRECTION_RIGHT;
    last_pressed_horiz = DIRECTION_RIGHT;
  }
  if (input & J_UP) {
    bg_pos_y--;
    direction |= DIRECTION_UP;
  }
  if (input & J_DOWN) {
    bg_pos_y++;
    direction |= DIRECTION_DOWN;
  }

  move_bkg(bg_pos_x & 0xFF, bg_pos_y & 0xFF);

  prev_col = prev_bg_pos_x >> 3;
  prev_row = prev_bg_pos_y >> 3;
  cur_col = bg_pos_x >> 3;
  cur_row = bg_pos_y >> 3;

  if (cur_col > prev_col) {
    set_aquaria_map_tile_column(cur_col + 20, cur_row, cur_col + 20);
    set_aquaria_map_attrib_column(cur_col + 20, cur_row, cur_col + 20);
  } else if (cur_col < prev_col) {
    set_aquaria_map_tile_column(cur_col, cur_row, cur_col);
    set_aquaria_map_attrib_column(cur_col, cur_row, cur_col);
  }

  if (cur_row > prev_row) {
    set_aquaria_map_tile_row(cur_row + 18, cur_row + 18, cur_col);
    set_aquaria_map_attrib_row(cur_row + 18, cur_row + 18, cur_col);
  } else if (cur_row < prev_row) {
    set_aquaria_map_tile_row(cur_row, cur_row, cur_col);
    set_aquaria_map_attrib_row(cur_row, cur_row, cur_col);
  }

  if(last_direction != direction) {
    update_player_sprite();
  }
}
