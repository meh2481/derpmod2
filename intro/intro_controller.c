#include <gb/gb.h>
#include "intro_controller.h"
#include "../utils/utils.h"
#include "../font/font_tiles.h"
#include "../aquaria/aquaria_controller.h"
#include "../title/press_start.h"

int8_t cur_string_char = 0;
uint8_t cur_string = 0;
uint8_t pressed_a = 0;

void init_intro(void) NONBANKED {
  gamestate = STATE_INTRO;

  set_press_start_text_palette(6);

  VBK_REG = VBK_BANK_1;
  set_font_tiles(0);  // Font tiles

  render_textbox_id(TEXT_STRING_TAKEOVER_WORLD, 0);
  VBK_REG = VBK_BANK_0;
}

void update_intro(uint8_t input) NONBANKED {
  if (input & J_A && !pressed_a) {
    pressed_a = 1;
    if (cur_string_char == -1) {
      cur_string++;
      if (cur_string == 6) {
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
