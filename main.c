/*
 * Derpmod2
 * A game by Daxar
 */
#include "hUGEDriver.h"

#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdint.h>
#include "utils/hUGEHelpers.h"
#include "utils/utils.h"
#include "aquaria/aquaria_controller.h"
#include "title/title_controller.h"
#include "intro/intro_controller.h"
#include "logos/logo_controller.h"
#include "sfx/cbtfx.h"

#define WIN_X_OFFSET 7

uint8_t i, j, tmp, tmp_counter;
uint8_t gamestate;

void main(void)
{
  disable_interrupts();
  set_default_palette();
  cpu_fast();
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

  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);

  // init_title();
  // init_aquaria();
  init_logo();

  DISPLAY_ON;
  enable_interrupts();

  while(1) {
    vsync();
    i = joypad();
    if (gamestate == STATE_START) {
      update_title_win(i);
    } else if (gamestate == STATE_PLAY) {
      update_aquaria(i);
    } else if (gamestate == STATE_INTRO) {
      update_intro(i);
    } else if (gamestate == STATE_LOGO) {
      update_logo(i);
    }
    // Update SFX after hUGE (updated in methods above) so sounds play over music
    CBTFX_update();
  }
}
