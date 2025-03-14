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
#include "vvvvvv_controller.h"
#include "../title/press_start.h"

#define NUM_SCREENS_X      7
#define NUM_SCREENS_Y      7
#define HALF_NUM_SCREENS_Y 4

#define PALETTE_FLIPLINES  1
#define PALETTE_SAVEPOINTS 2
#define PALETTE_MINIMAP    3
#define PALETTE_MAP_FOG    4
#define PALETTE_DISCO_BALL 5
#define PALETTE_TEXTBOX    6
#define PALETTE_COOL_TEXT  7

#define PLAYER_SPRITE      0
#define FALL_AMOUNT        4
#define PLAYER_MOVE_SPEED  2
#define PLAYER_ANIM_COUNT  4
#define PLAYER_HAS_GLASSES 4

#define WINDOW_MAP_X       95
#define WINDOW_MAP_Y       72

#define START_MINIMAP_TILES_IDX 94

#define MINIMAP_BLINK_AMOUNT 20

const uint8_t top_bot_mapbox_tiles[] = {
  0x5B, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5B
};

const uint8_t top_mapbox_attribs[] = {
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xA8
};

const uint8_t bottom_mapbox_attribs[] = {
  0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xE8
};

extern uint8_t i, j, tmp, tile;
uint16_t curScreenX;
uint16_t curScreenY;

uint16_t lastScreenX;
uint16_t lastScreenY;
uint16_t lastPlayerSpriteX;
uint16_t lastPlayerSpriteY;
uint8_t lastPlayerFlipped;

uint8_t mapMenu;
// Center of player sprite
int16_t playerSpriteX;
int16_t playerSpriteY;
uint8_t playerFlipped;
uint8_t playerMoveLeft;
uint8_t playerCanFlip;
uint8_t playerPressingA;
uint8_t playerMoveAnimDelay;
uint8_t playerAnimApplied;
uint8_t playerHasGlasses;
extern uint8_t display_dialog;
uint8_t cur_vvvvvv_dialogue;
uint8_t cur_vvvvvv_dialogue_start;
uint8_t cur_vvvvvv_dialogue_length;
extern int8_t cur_displaying_string_char;

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

void draw_window_box(void) {
  VBK_REG = VBK_TILES;
  init_win(0);
  // Set window to bank 1, map fog palette
  VBK_REG = VBK_ATTRIBUTES;
  for (i = 0; i < SCREEN_WIDTH_TILES; i++) {
    for (j = 0; j < SCREEN_HEIGHT_TILES; j++) {
      set_win_tile_xy(i, j, 8 | PALETTE_MAP_FOG);
    }
  }
  VBK_REG = VBK_TILES;

  // Draw top border
  VBK_REG = VBK_TILES;
  set_win_tiles(0, 0, 9, 1, top_bot_mapbox_tiles);
  VBK_REG = VBK_ATTRIBUTES;
  set_win_tiles(0, 0, 9, 1, top_mapbox_attribs);

  // Draw bottom border
  VBK_REG = VBK_TILES;
  set_win_tiles(0, 8, 9, 1, top_bot_mapbox_tiles);
  VBK_REG = VBK_ATTRIBUTES;
  set_win_tiles(0, 8, 9, 1, bottom_mapbox_attribs);

  // Draw left side
  for (i = 1; i < 8; i++) {
    tile = 93;
    VBK_REG = VBK_TILES;
    set_win_tiles(0, i, 1, 1, &tile);
    VBK_REG = VBK_ATTRIBUTES;
    tile = 0x88;
    set_win_tiles(0, i, 1, 1, &tile);
  }

  // Draw right side
  for (i = 1; i < 8; i++) {
    tile = 93;
    VBK_REG = VBK_TILES;
    set_win_tiles(8, i, 1, 1, &tile);
    VBK_REG = VBK_ATTRIBUTES;
    tile = 0xA8;
    set_win_tiles(8, i, 1, 1, &tile);
  }
  VBK_REG = VBK_TILES;
}

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

  // Add sprites for this screen
  add_vvvvvv_sprites(curScreenX, curScreenY);
}

uint8_t isOnGround = 0;

void init_vvvvvv(void) NONBANKED {
  gamestate = STATE_VVVVVV;
  init_vvvvvv_music();
  // Display the window covering everything
  init_win(0);
  move_win(WIN_X_OFFSET, 0);

  #ifndef DEBUG
  lastScreenX = curScreenX = 0;
  lastScreenY = curScreenY = 6;
  #else
  lastScreenX = curScreenX = 0;
  lastScreenY = curScreenY = 3;
  #endif
  cur_pressing_start = 0;
  mapMenu = 0;

  init_vvvvvv_tileset();
  VBK_REG = VBK_BANK_1;
  set_font_tiles(0);
  init_vvvvvv_minimap_tiles(START_MINIMAP_TILES_IDX+1);
  VBK_REG = VBK_BANK_0;

  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);

  draw_window_box();

  draw_screen();

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
  set_bkg_palette_entry(PALETTE_MAP_FOG, 0, RGB(10,10,10));
  set_bkg_palette_entry(PALETTE_DISCO_BALL, 0, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_DISCO_BALL, 1, RGB(31,31,31));
  set_bkg_palette_entry(PALETTE_DISCO_BALL, 2, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_DISCO_BALL, 3, RGB(0,0,0));
  set_bkg_palette_entry(PALETTE_COOL_TEXT, 0, RGB(31,0,0));

  set_press_start_text_palette(PALETTE_TEXTBOX);

  // Set sprite data
  init_vvvvvv_sprite_tiles();
  init_vvvvvv_sprite_palettes(0);

  SPRITES_8x16;
  lastPlayerFlipped = playerFlipped = playerCanFlip = playerMoveLeft = playerPressingA = 0;
  #ifdef DEBUG
  lastPlayerSpriteX = playerSpriteX = 144;
  lastPlayerSpriteY = playerSpriteY = 104;
  playerHasGlasses = 4;
  set_sprite_palette_entry(0, 3, RGB(0, 0, 0));
  #else
  lastPlayerSpriteX = playerSpriteX = 76;
  lastPlayerSpriteY = playerSpriteY = 24;
  playerHasGlasses = 0;
  #endif
  move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
}

uint8_t minimap_blink_counter = 0;
uint8_t minimap_blink_on = 1;

void update_vvvvvv(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(vvvvvv_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  // Handle dialog character-by-character rendering
  if (cur_displaying_string_char != -1) {
    // If A is pressed, display all text immediately
    if (input & J_A && !playerPressingA) {
      playerCanFlip = 0;
      playerPressingA = 1;
      // Show all text at once
      while (cur_displaying_string_char != -1) {
        cur_displaying_string_char = render_next_string_char_id(cur_vvvvvv_dialogue, cur_displaying_string_char, 0);
      }
    } else {
      // Display one character at a time
      cur_displaying_string_char = render_next_string_char_id(cur_vvvvvv_dialogue, cur_displaying_string_char, 0);
    }
  }

  // If dialog is complete and player presses A, dismiss it
  if (input & J_A && !playerPressingA && display_dialog && cur_displaying_string_char == -1) {
    playerPressingA = 1;
    if (++cur_vvvvvv_dialogue >= cur_vvvvvv_dialogue_start + cur_vvvvvv_dialogue_length) {
      display_dialog = 0;
      playerCanFlip = 0;
      move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
    } else {
      // Next line of text
      cur_displaying_string_char = 0;
      render_textbox_id(cur_vvvvvv_dialogue, 0);
    }
  }

  if (!(input & J_A)) {
    playerPressingA = 0;
  }

  // Only process other input if no dialog is showing
  if (!display_dialog) {
    if (input & J_START) {
      // Blink minimap
      minimap_blink_counter++;
      if (minimap_blink_counter > MINIMAP_BLINK_AMOUNT) {
        if (minimap_blink_on) {
          minimap_blink_on = 0;
          VBK_REG = VBK_ATTRIBUTES;
          set_win_tile_xy(curScreenX+1, curScreenY+1, 0x8);
          VBK_REG = VBK_TILES;
        } else {
          minimap_blink_on = 1;
          VBK_REG = VBK_ATTRIBUTES;
          set_win_tile_xy(curScreenX+1, curScreenY+1, 0x8 | PALETTE_MINIMAP);
          VBK_REG = VBK_TILES;
        }
        minimap_blink_counter = 0;
      }

      // Holding start shows the map
      if (!mapMenu) {
        draw_window_box();
        mapMenu = 1;
        // Display the window in the lower right
        move_win(WINDOW_MAP_X, WINDOW_MAP_Y);
        HIDE_SPRITES;
        // Draw the map
        for (i = 0; i < NUM_SCREENS_X; i++) {
          for (j = 0; j < NUM_SCREENS_Y; j++) {
            if (seenScreens[i][j]) {
              set_win_tile_xy(i+1, j+1, NUM_SCREENS_X * j + i + START_MINIMAP_TILES_IDX + 1);
              VBK_REG = VBK_ATTRIBUTES;
              if (curScreenX == i && curScreenY == j && minimap_blink_counter) {
                // Bank 1, palette for minimap
                set_win_tile_xy(i+1, j+1, 0x8 | PALETTE_MINIMAP);
              } else {
                // Bank 1
                set_win_tile_xy(i+1, j+1, 8);
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
      update_player(input);
    }
  }
}
