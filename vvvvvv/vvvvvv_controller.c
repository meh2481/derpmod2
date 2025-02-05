#include <gb/gb.h>
#include <rand.h>
#include "../utils/hUGEHelpers.h"
#include "vvvvvv_tiles.h"
#include "vvvvvv_map.h"
#include "vvvvvv_palettes.h"
#include "sprite_tileset_tiles.h"
#include "../utils/utils.h"
#include "../sfx/sfx.h"
#include "../font/font_tiles.h"

#define NUM_SCREENS_X      7
#define NUM_SCREENS_Y      7
#define HALF_NUM_SCREENS_Y 4

#define PALETTE_FLIPLINES  1
#define PALETTE_SAVEPOINTS 2

extern uint8_t i;
uint16_t curScreenX;
uint16_t curScreenY;

void draw_screen(void) {
  // Handle our different banks for portions of the map
  if (curScreenY > 3) {
    for (i = 0; i < SCREEN_HEIGHT_TILES; i++) {
      set_vvvvvv_map_tile_row2(SCREEN_HEIGHT_TILES * (curScreenY-4) + i, curScreenX * SCREEN_WIDTH_TILES, i);
      set_vvvvvv_map_attrib_row2(SCREEN_HEIGHT_TILES * (curScreenY-4) + i, curScreenX * SCREEN_WIDTH_TILES, i);
    }
  } else {
    for (i = 0; i < SCREEN_HEIGHT_TILES; i++) {
      set_vvvvvv_map_tile_row(SCREEN_HEIGHT_TILES * curScreenY + i, curScreenX * SCREEN_WIDTH_TILES, i);
      set_vvvvvv_map_attrib_row(SCREEN_HEIGHT_TILES * curScreenY + i, curScreenX * SCREEN_WIDTH_TILES, i);
    }
  }

  // Set the palette for this screen
  set_vvvvvv_room_palette(curScreenX + curScreenY * NUM_SCREENS_X);

  // Set the palette for objects (black bg, white obj)
  set_bkg_palette_entry(PALETTE_FLIPLINES, 0, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 1, RGB(31,31,31));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 2, RGB(18,10,21));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 3, RGB(10,6,13));
  set_bkg_palette_entry(PALETTE_SAVEPOINTS, 0, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_SAVEPOINTS, 1, RGB(31,31,31));

  // Set sprite data
  init_vvvvvv_sprite_tiles();
  init_vvvvvv_sprite_palettes(0);

  SPRITES_8x16;
  move_sprite(0, 80+8 - 4, 72+16 - 8);
}

void init_vvvvvv(void) NONBANKED {
  gamestate = STATE_VVVVVV;
  init_vvvvvv_music();
  // Display the window covering everything
  init_win(0);
  move_win(WIN_X_OFFSET, 0);

  curScreenX = curScreenY = 0;

  init_vvvvvv_tileset();

  draw_screen();

  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
}

uint8_t cur_pressing_arrow = 0;

void update_vvvvvv(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(vvvvvv_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  if (!cur_pressing_arrow) {
    if (input & J_LEFT) {
      if (curScreenX == 0) {
        curScreenX = NUM_SCREENS_X - 1;
      } else {
        curScreenX--;
      }
      draw_screen();
      cur_pressing_arrow = 1;
    } else if (input & J_RIGHT) {
      if (curScreenX == NUM_SCREENS_X - 1) {
        curScreenX = 0;
      } else {
        curScreenX++;
      }
      draw_screen();
      cur_pressing_arrow = 1;
    }
    if (input & J_UP) {
      if (curScreenY == 0) {
        curScreenY = NUM_SCREENS_Y - 1;
      } else {
        curScreenY--;
      }
      draw_screen();
      cur_pressing_arrow = 1;
    } else if (input & J_DOWN) {
      if (curScreenY == NUM_SCREENS_Y - 1) {
        curScreenY = 0;
      } else {
        curScreenY++;
      }
      draw_screen();
      cur_pressing_arrow = 1;
    }
  }

  if (!(input & J_LEFT) && !(input & J_RIGHT) && !(input & J_UP) && !(input & J_DOWN)) {
    cur_pressing_arrow = 0;
  }
}
