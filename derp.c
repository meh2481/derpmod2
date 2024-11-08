/*
 * Derpmod2
 * A game by Daxar
 */
#include "hUGEDriver.h"
#include "title/title.h"
#include "title/title_text.h"
#include "title/title_bg_tiles_hi.h"
#include "title/title_bg_tiles_low.h"
#include "title/press_start.h"
#include "font/font_tiles.h"

extern const hUGESong_t pressure_cooker;
extern const hUGESong_t space_odyssey;
extern const hUGESong_t the_traveller;

#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdint.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

#define STATE_START 0
#define STATE_PLAY  1

#define WIN_X_OFFSET 7

uint8_t bSwitchedSound = 0x0;
uint8_t i, j, tmp;
uint8_t titleTextPosY = 0;
uint8_t titleUpdateIndex = 0;
uint8_t gamestate = STATE_START;
uint8_t windowCounter = 0;
#define TITLE_VSYNC_FRAMES 3
#define NUM_TITLE_TEXT_TILES 54
uint8_t blinkStartText = 25;

void hUGE_stop_music(void) {
  // Not sure why this isn't part of the library; mute all channels
  hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH2, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
}

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
  if (windowCounter > TITLE_VSYNC_FRAMES) {
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
  }

  if (input & J_START) {
    gamestate = STATE_PLAY;
    // Stop title screen music
    hUGE_stop_music();
    // Start game music
    hUGE_init(&the_traveller);
  }
}

void main(void)
{
  disable_interrupts();
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_WIN9C00 | LCDCF_WINON | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_OBJ16 | LCDCF_OBJON | LCDCF_BGON;

  /*
   * LCD        = Off
   * WindowBank = 0x9C00
   * Window     = On
   * BG Chr     = 0x8800
   * BG Bank    = 0x9800
   * OBJ        = 8x16
   * OBJ        = On
   * BG         = On
   */

  // Init sound playback
  NR52_REG = 0x80;
  NR51_REG = 0xFF;
  NR50_REG = 0x77;

  init_title();

  DISPLAY_ON;
  enable_interrupts();

  while(1) {
    vsync();
    i = joypad();
    if (gamestate == STATE_START) {
      update_title_win(i);
    }
    hUGE_dosound();
  }
}
