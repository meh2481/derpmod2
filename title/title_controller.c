#include "hUGEDriver.h"
#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <stdlib.h>
#include <rand.h>
#include "title.h"
#include "title_text.h"
#include "title_bg_tiles_hi.h"
#include "title_bg_tiles_low.h"
#include "press_start.h"
#include "../font/font_tiles.h"
#include "../utils/hUGEHelpers.h"
#include "../utils/utils.h"
#include "../intro/intro_controller.h"
#include "../sfx/sfx.h"

#define TITLE_VSYNC_FRAMES 3
#define NUM_TITLE_TEXT_TILES 54

uint8_t blinkStartText = 25;
uint8_t titleTextPosY = 0;
uint8_t titleUpdateIndex = 0;
uint8_t windowCounter = 0;
palette_color_t* title_fadeout_palette;
uint8_t fadeValue = 32;
palette_color_t tmpPal;
uint8_t tempR, tempG, tempB;

void init_title(void) NONBANKED {
  gamestate = STATE_START;
  set_title_bg_tiles_low_palette(); // 2 bg palettes
  set_press_start_text_palette(2);

  // Display the window covering everything
  move_win(WIN_X_OFFSET, 0);

  // Initialize the background tiles
  set_title_bg_tiles_low();
  set_title_bg_tiles_hi();
  VBK_REG = VBK_BANK_1;
  set_font_tiles(353 - 256);  // Font tiles
  VBK_REG = VBK_BANK_0;

  // Draw the background
  set_title_bg_screen();

  // Set window attributes
  // Second palette for full black
  VBK_REG = VBK_ATTRIBUTES;
  tmp = 0x1;
  for (i = 0; i < titleHeight; i++) {
    for (j = 0; j < titleWidth; j++) {
      set_win_tiles(j, i, 1, 1, &tmp);
    }
  }
  VBK_REG = VBK_TILES;

  SCX_REG = 0;
  SCY_REG = 0;

  // Start title screen music
  init_title_music();
}

void update_title_win(uint8_t input) NONBANKED {

  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(title_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  windowCounter++;
  if (windowCounter > TITLE_VSYNC_FRAMES && fadeValue == 32) {
    windowCounter = 0;
    if(titleTextPosY < SCREEN_HEIGHT) {
      titleTextPosY++;
      move_win(WIN_X_OFFSET, titleTextPosY);
    } else if (titleUpdateIndex < NUM_TITLE_TEXT_TILES) {
      // Display the title one tile at a time
      show_title_tile(titleUpdateIndex);
      titleUpdateIndex++;
    } else {
      // Blink the "Press Start" text
      if (blinkStartText % 30 < 15) {
        show_press_start_text();
      } else {
        // Reset to the background
        hide_press_start_text();
      }
      blinkStartText++;
      if (blinkStartText > 30) {
        blinkStartText = 1;
      }
    }
  } else if (fadeValue < 32) {
    fadeValue--;
    if (fadeValue == 0) {
      free(title_fadeout_palette);
      fadeValue = 32;
      // Start intro
      init_intro();
      return;
    } else {
      for (i = 0; i < 12; i++) {
        tmpPal = title_fadeout_palette[i];
        tempR = tmpPal & 0x1F;
        tempG = (tmpPal >> 5) & 0x1F;
        tempB = (tmpPal >> 10) & 0x1F;
        if (tempR > 0) {
          tempR--;
        }
        if (tempG > 0) {
          tempG--;
        }
        if (tempB > 0) {
          tempB--;
        }
        title_fadeout_palette[i] = tempR | (tempG << 5) | (tempB << 10);
      }
      set_bkg_palette(0, 3, title_fadeout_palette);
    }
  }

  if (input & J_START && fadeValue == 32) {
    // Init the random number generator
    uint16_t seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);
    // Reset fade timer
    fadeValue = 31;
    // Stop music
    hUGE_stop_music();
    // Play start button sound
    CBTFX_PLAY_SFX_START_BUTTON;
    // Store palette info for fadeout
    title_fadeout_palette = (palette_color_t*) malloc(24);
    for (i = 0; i < 8; i++) {
      title_fadeout_palette[i] = get_title_bg_tiles_low_palette(i);
    }
    for (i = 8, j = 0; i < 12; i++, j++) {
      title_fadeout_palette[i] = get_press_start_palette(j);
    }
  }
}
