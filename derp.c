/*
 * Derpmod2
 * A game by Daxar
 */
#include "hUGEDriver.h"
#include "tiles/title.h"
#include "tiles/title_text.h"
#include "tiles/title_bg_tiles_hi.h"
#include "tiles/title_bg_tiles_low.h"

extern const hUGESong_t pressure_cooker;
extern const hUGESong_t space_odyssey;
extern const hUGESong_t the_traveller;

#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdint.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

uint8_t bSwitchedSound = 0x0;
uint8_t i, j, tmp;
uint8_t titleTextPosY = 0;

void update_win() {
  if(titleTextPosY < 144) {
    titleTextPosY++;
    move_win(7, titleTextPosY);
  } else {
    move_win(7, MAXWNDPOSY + 1);
  }
}

void main(void)
{
  disable_interrupts();
  DISPLAY_OFF;
  cpu_fast();
  // set_sprite_palette(0, 1, spritePaletteData);
  set_bkg_palette(0, 2, title_bg_tiles_lowCGBPal);  // 2 bg palettes
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

  NR52_REG = 0x80;
  NR51_REG = 0xFF;
  NR50_REG = 0x77;

  // Initialize the background tiles
  VBK_REG = VBK_BANK_0;
  gb_decompress_bkg_data(0, title_bg_tiles_low);

  VBK_REG = VBK_BANK_1;
  gb_decompress_bkg_data(0, title_bg_tiles_hi);
  VBK_REG = VBK_BANK_0;

  // Draw the background
  set_bkg_tiles(0, 0, titleWidth, titleHeight - 5, titleBLK0PLN0);  // Top 12 rows
  set_bkg_tiles(titleWidth - 4, titleHeight - 6, 4, 1, titleBLK1PLN0);  // Final four tiles in row 12
  set_bkg_tiles(0, titleHeight - 5, titleWidth, 5, &titleBLK1PLN0[4]);  // Bottom 6 rows
  // Set window tiles
  // set_win_tiles(0, 0, title_textWidth, title_textHeight, title_textPLN0);
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
    // set_win_tiles(i, 0, 1, title_textHeight, &title_textPLN1[i * title_textHeight]);
  }
  // set_win_tiles(0, 0, title_textWidth, title_textHeight, title_textPLN1);



  SCX_REG = 0;
  SCY_REG = 0;

  // Hide the window
  WX_REG = MAXWNDPOSX;
  WY_REG = MAXWNDPOSY;

  DISPLAY_ON;
  enable_interrupts();

  // Start music
  // hUGE_init(&space_odyssey);

  while(1) {
    vsync();
    update_win();
    // hUGE_dosound();
    i = joypad();

    if(i & J_SELECT) {
      if(bSwitchedSound == 0x0) {
        if(i & J_B) {
          hUGE_init(&pressure_cooker);
        } else if (i & J_A) {
          hUGE_init(&the_traveller);
        } else {
          hUGE_init(&space_odyssey);
        }
        bSwitchedSound = 0x1;
      }
    } else {
      bSwitchedSound = 0x0;
    }
  }
}
