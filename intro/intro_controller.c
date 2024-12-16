#include <gb/gb.h>
#include "intro_controller.h"
#include "../utils/utils.h"
#include "../font/font_tiles.h"
#include "../aquaria/aquaria_controller.h"
#include "../title/press_start.h"
#include "../utils/hUGEHelpers.h"
#include "mia_hug_map.h"
#include "mia_hug_tiles.h"
#include "mia_brainwash_tiles.h"
#include "mia_brainwash_map.h"
#include "naija_crystal_tiles.h"
#include "naija_crystal_map.h"
#include "fishform_gasp_tiles.h"
#include "fishform_gasp_map.h"

int8_t cur_string_char = 0;
uint8_t cur_string = 0;
uint8_t pressed_a = 0;

void init_intro(void) NONBANKED {
  gamestate = STATE_INTRO;
  init_pajama_music();

  // Display the window covering everything
  move_win(WIN_X_OFFSET, 0);

  init_mia_brainwash_tiles(0, 0);
  draw_mia_brainwash_bg(4, 0);

  set_press_start_text_palette(6);

  VBK_REG = VBK_BANK_1;
  set_font_tiles(0);  // Font tiles

  render_textbox_id(TEXT_STRING_TAKEOVER_WORLD, 0);
  VBK_REG = VBK_BANK_0;
}

void update_intro(uint8_t input) NONBANKED {
  // Switch to pajama music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(pajama_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  if (input & J_A && !pressed_a) {
    pressed_a = 1;
    if (cur_string_char == -1) {
      cur_string++;
      switch (cur_string) {
        case 1:
          init_mia_hug_tiles(0, 0);
          draw_mia_hug_bg(6, 0);
          break;
        case 2:
          VBK_REG = VBK_TILES;
          init_bkg(175);
          VBK_REG = VBK_ATTRIBUTES;
          init_bkg(1);
          break;
        case 3:
          init_naija_crystal_tiles(0, 0);
          draw_naija_crystal_bg(5, 0);
          break;
        case 4:
          VBK_REG = VBK_TILES;
          init_bkg(175);
          VBK_REG = VBK_ATTRIBUTES;
          init_bkg(1);
          break;
        case 5:
          break;
        case 6:
          init_fishform_gasp_tiles(0, 0);
          draw_fishform_gasp_bg(4, 2);
          break;
        case 7:
          hUGE_stop_music();
          gamestate = STATE_PLAY;
          init_aquaria();
          return;
      }
      cur_string_char = 0;
      render_textbox_id(cur_string, 0);
    } else {
      // Render the rest of the string
      while (cur_string_char != -1) {
        cur_string_char = render_next_string_char_id(cur_string, cur_string_char, 0);
      }
    }
  } else if (!(input & J_A)) {
    pressed_a = 0;
    cur_string_char = render_next_string_char_id(cur_string, cur_string_char, 0);
  }
}
