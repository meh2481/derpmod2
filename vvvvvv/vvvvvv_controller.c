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
#include "minimap_tiles.h"

#define NUM_SCREENS_X      7
#define NUM_SCREENS_Y      7
#define HALF_NUM_SCREENS_Y 4

#define PALETTE_FLIPLINES  1
#define PALETTE_SAVEPOINTS 2
#define PALETTE_MINIMAP    3

#define MINIMAP_BLINK_AMOUNT 20

extern uint8_t i, j, tmp;
uint16_t curScreenX;
uint16_t curScreenY;
uint8_t mapMenu;

uint8_t cur_pressing_arrow;
uint8_t cur_pressing_start;

uint8_t seenScreens[7][7] = {
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0}
};

void draw_screen(void) {
  seenScreens[curScreenX][curScreenY] = 1;

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

  // Set the palette for objects (black bg, white obj, blue minimap)
  set_bkg_palette_entry(PALETTE_FLIPLINES, 0, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 1, RGB(31,31,31));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 2, RGB(18,10,21));
  set_bkg_palette_entry(PALETTE_FLIPLINES, 3, RGB(10,6,13));
  set_bkg_palette_entry(PALETTE_SAVEPOINTS, 0, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_SAVEPOINTS, 1, RGB(31,31,31));
  set_bkg_palette_entry(PALETTE_MINIMAP, 0, RGB(0,10,10));
  set_bkg_palette_entry(PALETTE_MINIMAP, 1, RGB(0,31,31));
  set_bkg_palette_entry(PALETTE_MINIMAP, 2, RGB(0,25,25));
  set_bkg_palette_entry(PALETTE_MINIMAP, 3, RGB(0,15,15));

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
  cur_pressing_arrow = 0;
  cur_pressing_start = 0;
  mapMenu = 0;

  init_vvvvvv_tileset();
  VBK_REG = VBK_BANK_1;
  init_vvvvvv_minimap_tiles(1);
  VBK_REG = VBK_BANK_0;

  VBK_REG = VBK_ATTRIBUTES;
  for (i = 0; i < SCREEN_WIDTH_TILES; i++) {
    for (j = 0; j < SCREEN_HEIGHT_TILES; j++) {
      // Set to bank 1
      set_win_tile_xy(i, j, 8);
    }
  }
  VBK_REG = VBK_TILES;

  draw_screen();

  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
}

uint8_t minimap_blink_counter = 0;
uint8_t minimap_blink_on = 1;

void update_vvvvvv(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(vvvvvv_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  if (!(input & J_LEFT) && !(input & J_RIGHT) && !(input & J_UP) && !(input & J_DOWN)) {
    cur_pressing_arrow = 0;
  }

  if (input & J_START) {
    // Blink minimap
    minimap_blink_counter++;
    if (minimap_blink_counter > MINIMAP_BLINK_AMOUNT) {
      if (minimap_blink_on) {
        minimap_blink_on = 0;
        VBK_REG = VBK_ATTRIBUTES;
        set_win_tile_xy(curScreenX, curScreenY, 0x8);
        VBK_REG = VBK_TILES;
      } else {
        minimap_blink_on = 1;
        VBK_REG = VBK_ATTRIBUTES;
        set_win_tile_xy(curScreenX, curScreenY, 0x8 | PALETTE_MINIMAP);
        VBK_REG = VBK_TILES;
      }
      minimap_blink_counter = 0;
    }

    // Holding start shows the map
    if (!mapMenu) {
      mapMenu = 1;
      // Display the window in the lower right
      move_win(111, 88);
      HIDE_SPRITES;
      // Draw the map
      for (i = 0; i < NUM_SCREENS_X; i++) {
        for (j = 0; j < NUM_SCREENS_Y; j++) {
          if (seenScreens[i][j]) {
            set_win_tile_xy(i, j, NUM_SCREENS_X * j + i + 1);
            VBK_REG = VBK_ATTRIBUTES;
            if (curScreenX == i && curScreenY == j && minimap_blink_counter) {
              // Bank 1, palette for minimap
              set_win_tile_xy(i, j, 0x8 | PALETTE_MINIMAP);
            } else {
              // Bank 1
              set_win_tile_xy(i, j, 8);
            }
            VBK_REG = VBK_TILES;
          }
        }
      }
    }
  } else {
    minimap_blink_counter = 0;
    minimap_blink_on = 1;
    if (mapMenu) {
      mapMenu = 0;
      move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
      SHOW_SPRITES;
    }

    // Only allow moving if map is off
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
  }
}
