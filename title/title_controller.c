#include "hUGEDriver.h"
#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <stdlib.h>
#include "title.h"
#include "title_text.h"
#include "title_bg_tiles_hi.h"
#include "title_bg_tiles_low.h"
#include "press_start.h"
#include "../font/font_tiles.h"
#include "../utils/hUGEHelpers.h"
#include "../utils/utils.h"

#define WIN_X_OFFSET 7
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

void init_title(void) {
  set_bkg_palette(0, 2, title_bg_tiles_lowCGBPal);  // 2 bg palettes
  set_bkg_palette(2, 1, press_start_textCGBPal);

  // Initialize the background tiles
  VBK_REG = VBK_BANK_0;
  gb_decompress_bkg_data(0, title_bg_tiles_low);

  VBK_REG = VBK_BANK_1;
  gb_decompress_bkg_data(0, title_bg_tiles_hi);
  gb_decompress_bkg_data(353 - 256, font_tiles);  // Font tiles
  VBK_REG = VBK_BANK_0;

  // Draw the background
  VBK_REG = VBK_TILES;
  set_bkg_tiles(0, 0, titleWidth, titleHeight - 5, titleBLK0PLN0);  // Top 12 rows
  set_bkg_tiles(titleWidth - 4, titleHeight - 6, 4, 1, titleBLK1PLN0);  // Final four tiles in row 12
  set_bkg_tiles(0, titleHeight - 5, titleWidth, 5, &titleBLK1PLN0[4]);  // Bottom 6 rows
  // Set bg attributes
  VBK_REG = VBK_ATTRIBUTES;
  set_bkg_tiles(0, 0, titleWidth, titleHeight - 5, titleBLK0PLN1);  // Top 12 rows
  set_bkg_tiles(titleWidth - 4, titleHeight - 6, 4, 1, titleBLK1PLN1);  // Final four tiles in row 12
  set_bkg_tiles(0, titleHeight - 5, titleWidth, 5, &titleBLK1PLN1[4]);  // Bottom 6 rows
  // Set window attributes
  // Second palette for full black
  tmp = 0x1;
  for (i = 0; i < titleHeight; i++) {
    for (j = 0; j < titleWidth; j++) {
      VBK_REG = VBK_ATTRIBUTES;
      set_win_tiles(j, i, 1, 1, &tmp);
    }
  }
  VBK_REG = VBK_TILES;

  SCX_REG = 0;
  SCY_REG = 0;

  // Start title screen music
  hUGE_init(&space_odyssey);
}

void update_title_win(uint8_t input) {
  windowCounter++;
  if (windowCounter > TITLE_VSYNC_FRAMES && fadeValue == 32) {
    windowCounter = 0;
    if(titleTextPosY < SCREEN_HEIGHT) {
      titleTextPosY++;
      move_win(WIN_X_OFFSET, titleTextPosY);
    } else if (titleUpdateIndex < NUM_TITLE_TEXT_TILES) {
      // Display the title one tile at a time
      VBK_REG = VBK_TILES;
      set_bkg_tiles(1 + titleUpdateIndex % title_textWidth, 1+titleUpdateIndex / title_textWidth, 1, 1, &title_text[titleUpdateIndex]);
      VBK_REG = VBK_ATTRIBUTES;
      set_bkg_tiles(1 + titleUpdateIndex % title_textWidth, 1+titleUpdateIndex / title_textWidth, 1, 1, &title_textPLN1[titleUpdateIndex]);
      titleUpdateIndex++;
    } else {
      // Blink the "Press Start" text
      if (blinkStartText % 30 < 15) {
        VBK_REG = VBK_TILES;
        set_bkg_tiles(3, 15, press_start_textWidth, press_start_textHeight, press_start_text);
        VBK_REG = VBK_ATTRIBUTES;
        set_bkg_tiles(3, 15, press_start_textWidth, press_start_textHeight, press_start_textPLN1);
      } else {
        // Reset to the background
        VBK_REG = VBK_TILES;
        set_bkg_tiles(3, 15, press_start_textWidth, 1, &titleBLK1PLN0[4 + 3 + 2 * titleWidth]);
        set_bkg_tiles(3, 16, press_start_textWidth, 1, &titleBLK1PLN0[4 + 3 + 3 * titleWidth]);
        set_bkg_tiles(3, 17, press_start_textWidth, 1, &titleBLK1PLN0[4 + 3 + 4 * titleWidth]);
        VBK_REG = VBK_ATTRIBUTES;
        set_bkg_tiles(3, 15, press_start_textWidth, 1, &titleBLK1PLN1[4 + 3 + 2 * titleWidth]);
        set_bkg_tiles(3, 16, press_start_textWidth, 1, &titleBLK1PLN1[4 + 3 + 3 * titleWidth]);
        set_bkg_tiles(3, 17, press_start_textWidth, 1, &titleBLK1PLN1[4 + 3 + 4 * titleWidth]);
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
      // Hide window
      move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
      // Start game
      gamestate = STATE_PLAY;
      hUGE_init(&the_traveller);
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
    // Reset fade timer
    fadeValue = 31;
    // Stop music
    hUGE_stop_music();
    // Store palette info for fadeout
    title_fadeout_palette = (palette_color_t*) malloc(24);
    for (i = 0; i < 8; i++) {
      title_fadeout_palette[i] = (palette_color_t*)(title_bg_tiles_lowCGBPal)[i];
    }
    for (i = 8, j = 0; i < 12; i++, j++) {
      title_fadeout_palette[i] = (palette_color_t*)(press_start_textCGBPal)[j];
    }
  }
}
