#include <gb/gb.h>
#include "logo_controller.h"
#include "../utils/utils.h"
#include "../title/title_controller.h"
#include "discord_logo_tiles.h"
#include "discord_logo_map.h"
#include "game_by_daxar_logo_tiles.h"
#include "game_by_daxar_logo_map.h"

extern uint8_t pressed_a;

void init_logo(void) NONBANKED {
  gamestate = STATE_LOGO;
  pressed_a = 0;

  // Display the window covering everything
  move_win(WIN_X_OFFSET, 0);

  // Initialize the background tiles
  init_game_by_daxar_logo_tiles(0, 0);
  draw_game_by_daxar_logo_bg(2, 5);

  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
}

uint8_t logoCounter = 0;

void update_logo(uint8_t input) NONBANKED {
  if (input & J_A || input & J_START) {
    if (!pressed_a) {
      pressed_a = 1;
      logoCounter++;
      if (logoCounter == 1) {
        VBK_REG = VBK_TILES;
        init_win(0);
        VBK_REG = VBK_ATTRIBUTES;
        init_win(1);
        VBK_REG = VBK_TILES;
        move_win(WIN_X_OFFSET, 0);
        init_discord_logo_tiles(0, 0);
        draw_discord_logo_bg(0, 3);
        move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
      } else if (logoCounter == 2) {
        pressed_a = 0;
        init_title();
        return;
      }
    }
  } else {
    pressed_a = 0;
  }
}
