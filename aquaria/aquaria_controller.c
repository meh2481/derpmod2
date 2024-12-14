#include <gb/gb.h>
#include <rand.h>
#include "../utils/hUGEHelpers.h"
#include "tileset.h"
#include "map1_tiles.h"
#include "../utils/utils.h"
#include "../sprite/naija_sprites.h"
#include "../sfx/sfx.h"
#include "../font/font_tiles.h"
#include "../title/press_start.h"

// Coordinates of note bulbs, in tiles
#define NOTE_BULB_X_1               47
#define NOTE_BULB_Y_1               19
#define NOTE_BULB_X_2               82
#define NOTE_BULB_Y_2               70
#define NOTE_BULB_X_3               20
#define NOTE_BULB_Y_3               89

#define NOTE_BULB_1                 1
#define NOTE_BULB_2                 2
#define NOTE_BULB_3                 4
uint8_t bulbs_active = NOTE_BULB_1 | NOTE_BULB_2 | NOTE_BULB_3;

#define WIN_X_OFFSET                7
#define PLAYER_SPRITE               0
#define DEFAULT_PLAYER_SPRITE_PROP  0
#define MAX_NOTE_SEQUENCE_LENGTH    8
#define NOTE_FRAMES_LENGTH          0x7F

uint8_t moved_bg = 1;
uint16_t bg_pos_x = 32;
uint16_t bg_pos_y = 128;
uint8_t direction = 4;
uint8_t last_direction = 4;
uint8_t last_pressed_horiz = 0;
#define DIRECTION_LEFT  1
#define DIRECTION_UP    2
#define DIRECTION_RIGHT 4
#define DIRECTION_DOWN  8

uint8_t cur_note = 0;
uint8_t last_note = 0;
uint8_t note_length_counter = 0;
uint16_t prev_bg_pos_x = 0;
uint16_t prev_bg_pos_y = 0;

#define BULB_SING_TIMER 0xB4  // 3 secs
#define SING_BULB_DISTANCE 36
uint8_t bulb_colors[] = {0, 0, 0};
uint8_t bulbs_open[] = {0, 0, 0};
uint8_t bulb_onscreen = 0;
uint8_t orig_bulb_tile = 0;
uint8_t vibrate_bulb_counter = 0;
uint8_t onscreen_bulb_x = 0;
uint8_t onscreen_bulb_y = 0;
// Note to palette (bulb colors above)
const uint8_t note_palette_remap[] = {1, 2, 0, 4, 3, 5, 7, 6};
const uint8_t* text_string = "My Mom told me all sorts of things. \"Let\'s take over the world!\", \"You must murder your husband!\", that sort of thing.";

uint8_t prev_col, prev_row, cur_col, cur_row;

int8_t vibrate_note_counter = 0;

uint8_t note_sequence[] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t bulb_palette_from_num(uint8_t rand_num) {
  if (rand_num < 3) {
    return 0x1;
  } else if (rand_num < 6) {
    return 0x2;
  } else {
    return 0x3;
  }
}

uint8_t bulb_tile_from_num(uint8_t rand_num) {
  if (rand_num < 3) {
    rand_num -= 0;
  } else if (rand_num < 6) {
    rand_num -= 3;
  } else {
    rand_num -= 6;
  }
  return rand_num % 3 + 12;
}

void setup_sprites(void) {
  SPRITES_8x8;
  set_fishform_sprite_tiles(0);
  set_fishform_sprite_palettes(0);

  // Create player sprite, facing right
  set_sprite_tile(PLAYER_SPRITE, 0);
  set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);

  // Create song note sprites
  tmp = 0x1; // Current sprite palette
  tmp_counter = 0;
  for(i = 0; i < 8; i++) {
    set_sprite_tile(i + 1, i + 4);
    set_sprite_prop(i + 1, tmp);
    tmp_counter++;
    // Every 3 sprites, change palette
    if (tmp_counter == 3) {
      tmp++;
      tmp_counter = 0;
    }
  }

  // Create note bulb sprites
  // Set note bulb sprite palettes randomized and store their colors
  uint8_t rand_num = rand() & 0b111;
  set_sprite_prop(9, bulb_palette_from_num(rand_num));
  set_sprite_tile(9, bulb_tile_from_num(rand_num));
  bulb_colors[0] = rand_num;
  rand_num = rand() & 0b111;
  set_sprite_prop(10, bulb_palette_from_num(rand_num));
  set_sprite_tile(10, bulb_tile_from_num(rand_num));
  bulb_colors[1] = rand_num;
  rand_num = rand() & 0b111;
  set_sprite_prop(11, bulb_palette_from_num(rand_num));
  set_sprite_tile(11, bulb_tile_from_num(rand_num));
  bulb_colors[2] = rand_num;

  // Center player sprite on the screen
  move_sprite(0, 84, 84);
}

void update_bulb_sprite(uint8_t sprite_num, int8_t move_x, int8_t move_y, uint8_t bulb_x, uint8_t bulb_y) {
  // Update song bulb sprites as the background moves
  uint8_t bg_pos_x_tile = bg_pos_x >> 3;
  uint8_t bg_pos_y_tile = bg_pos_y >> 3;
  if (bg_pos_x_tile <= bulb_x && bg_pos_y_tile <= bulb_y && bg_pos_x_tile >= bulb_x - SCREEN_WIDTH_TILES && bg_pos_y_tile >= bulb_y - SCREEN_HEIGHT_TILES) {
    onscreen_bulb_x = SCREEN_WIDTH - (((bg_pos_x_tile - bulb_x + 20) << 3) + (bg_pos_x & 0b111)) + 8 + move_x;
    onscreen_bulb_y = SCREEN_HEIGHT - (((bg_pos_y_tile - bulb_y + 18) << 3) + (bg_pos_y & 0b111)) + 16 + move_y;
    move_sprite(
      sprite_num,
      onscreen_bulb_x,
      onscreen_bulb_y
    );
    if (bulb_onscreen == 0 && bulbs_open[sprite_num - 9] == 0) {
      bulb_onscreen = sprite_num;
      orig_bulb_tile = get_sprite_tile(sprite_num);
    }
  } else {
    // Hide the sprite if it's off the screen
    hide_sprite(sprite_num);
    if (bulb_onscreen == sprite_num) {
      bulb_onscreen = 0;
    }
  }
}

void update_sprite_positions(int8_t move_x, int8_t move_y) {
  // If the note bulbs are on the screen, move them with the background
  if (bulbs_active & NOTE_BULB_1 && bulbs_open[0] == 0) {
    update_bulb_sprite(9, move_x, move_y, NOTE_BULB_X_1, NOTE_BULB_Y_1);
  }
  if (bulbs_active & NOTE_BULB_2 && bulbs_open[1] == 0) {
    update_bulb_sprite(10, move_x, move_y, NOTE_BULB_X_2, NOTE_BULB_Y_2);
  }
  if (bulbs_active & NOTE_BULB_3 && bulbs_open[2] == 0) {
    update_bulb_sprite(11, move_x, move_y, NOTE_BULB_X_3, NOTE_BULB_Y_3);
  }
}

void insert_note_into_sequence(uint8_t note) {
  for (i = 0; i < MAX_NOTE_SEQUENCE_LENGTH; i++) {
    if (note_sequence[i] == 0) {
      // Don't duplicate notes
      if (i > 0 && note_sequence[i - 1] == note) {
        return;
      }
      note_sequence[i] = note;
      return;
    }
  }

  // Note sequence buffer full; shift all notes to the left and insert into the end
  for (i = 0; i < MAX_NOTE_SEQUENCE_LENGTH - 1; i++) {
    note_sequence[i] = note_sequence[i + 1];
  }
  note_sequence[MAX_NOTE_SEQUENCE_LENGTH - 1] = note;

  // TODO: Check note sequence against possible songs
}

void stop_note(void) {
  // Stop channel that the notes play on
  hUGE_mute_channel(HT_CH2, HT_CH_MUTE);
  last_note = 0;
  cur_note = 0;
  // Reset bulb animation if it hasn't broken yet
  if (bulb_onscreen && bulbs_open[bulb_onscreen - 9] == 0) {
    set_sprite_tile(bulb_onscreen, orig_bulb_tile);
  }
}

void update_note(void) {
  if (cur_note == 0) {
    for (i = 0; i < MAX_NOTE_SEQUENCE_LENGTH; i++) {
      note_sequence[i] = 0;
    }
    return;
  }
  uint8_t note_sprite = 0;
  if (cur_note == DIRECTION_DOWN) {
    note_sprite = 1;
  } else if (cur_note == (DIRECTION_DOWN | DIRECTION_RIGHT)) {
    note_sprite = 2;
  } else if (cur_note == DIRECTION_RIGHT) {
    note_sprite = 3;
  } else if (cur_note == (DIRECTION_RIGHT | DIRECTION_UP)) {
    note_sprite = 4;
  } else if (cur_note == DIRECTION_UP) {
    note_sprite = 5;
  } else if (cur_note == (DIRECTION_UP | DIRECTION_LEFT)) {
    note_sprite = 6;
  } else if (cur_note == DIRECTION_LEFT) {
    note_sprite = 7;
  } else { // cur_note == (DIRECTION_LEFT | DIRECTION_DOWN)
    note_sprite = 8;
  }
  if(last_note != cur_note) {
    note_length_counter = 0;
    CBTFX_init(&(NOTE_LIST[note_sprite - 1][0]));
  } else {
    note_length_counter++;
    if (note_length_counter >= NOTE_FRAMES_LENGTH) {
      // Restart this note after it ends if we're still holding the button
      note_length_counter = 0;
      CBTFX_init(&(NOTE_LIST[note_sprite - 1][0]));
    }
  }
  insert_note_into_sequence(note_sprite - 1);

  // Shake the current note back and forth as you sing it
  if (vibrate_note_counter == 0
    || vibrate_note_counter == 1
    || vibrate_note_counter == 6
    || vibrate_note_counter == 7) {
    scroll_sprite(note_sprite, 1, 0);
  } else if (vibrate_note_counter == 2
    || vibrate_note_counter == 3
    || vibrate_note_counter == 4
    || vibrate_note_counter == 5) {
    scroll_sprite(note_sprite, -1, 0);
  } else if (vibrate_note_counter == 8) {
    vibrate_note_counter = -1;
  }
  vibrate_note_counter++;

  // Shake the current bulb if there's one onscreen (only if singing correct note)
  if (bulb_onscreen && bulbs_open[bulb_onscreen - 9] == 0 && note_palette_remap[note_sprite - 1] == bulb_colors[bulb_onscreen - 9]) {
    // Only if in range
    int8_t distance_x = (int8_t)(84) - ((int8_t)(onscreen_bulb_x));
    if (distance_x < 0) {
      distance_x = -distance_x;
    }
    int8_t distance_y = (int8_t)(84) - ((int8_t)(onscreen_bulb_y));
    if (distance_y < 0) {
      distance_y = -distance_y;
    }
    if (distance_x < SING_BULB_DISTANCE && distance_y < SING_BULB_DISTANCE) {
      if (vibrate_bulb_counter & 0b100) {
        set_sprite_tile(bulb_onscreen, orig_bulb_tile + 3);
      } else {
        set_sprite_tile(bulb_onscreen, orig_bulb_tile);
      }
      vibrate_bulb_counter++;
      if (vibrate_bulb_counter > BULB_SING_TIMER) {
        // Open bulb (TODO: Drops stuff)
        vibrate_bulb_counter = 0;
        bulbs_open[bulb_onscreen - 9] = 1;
        hide_sprite(bulb_onscreen);
        bulb_onscreen = 0;
        CBTFX_PLAY_SFX_OPEN_PLANT;
      }
    }
  } else if (bulb_onscreen && bulbs_open[bulb_onscreen - 9] == 0 && note_palette_remap[note_sprite - 1] != bulb_colors[bulb_onscreen - 9]) {
    vibrate_bulb_counter = 0;
    set_sprite_tile(bulb_onscreen, orig_bulb_tile);
  } else {
    vibrate_bulb_counter = 0;
  }
}

void show_song_note_sprites(void) {
  // Spread song note sprites out in a circle around the player sprite
  move_sprite(1, 84, 108);
  move_sprite(2, 101, 101);
  move_sprite(3, 108, 84);
  move_sprite(4, 101, 67);
  move_sprite(5, 84, 60);
  move_sprite(6, 67, 67);
  move_sprite(7, 60, 84);
  move_sprite(8, 67, 101);
}

void hide_song_note_sprites(void) {
  for(i = 1; i < 9; i++) {
    hide_sprite(i);
  }
}

void init_aquaria(void) NONBANKED {
  gamestate = STATE_PLAY;
  init_aquaria_music();
  set_aquaria_map_tiles();
  set_aquaria_map_tile_attribs();

  // Setup sprite data
  setup_sprites();

  init_aquaria_tileset();
  set_press_start_text_palette(6);

  VBK_REG = VBK_BANK_1;
  set_font_tiles(0);  // Font tiles

  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);

  VBK_REG = VBK_BANK_1;
  // render_string("For as long as I can remember, I came here to think about beans.", 0);
  // render_textbox(text_string, 0);
  render_textbox_id(TEXT_STRING_TAKEOVER_WORLD, 0);
  // render_string("My Mom told me all sorts of things. \"Let\'s take over the world!\", \"You must murder your husband!\", that sort of thing.", 0);
  VBK_REG = VBK_BANK_0;
}

void update_player_sprite(void) NONBANKED {
  if (direction == DIRECTION_LEFT) {
    set_sprite_tile(PLAYER_SPRITE, 0);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == DIRECTION_RIGHT) {
    set_sprite_tile(PLAYER_SPRITE, 0);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  } else if (direction == DIRECTION_UP) {
    set_sprite_tile(PLAYER_SPRITE, 1);
    if(last_pressed_horiz & DIRECTION_RIGHT) {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
    } else {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
    }
  } else if (direction == DIRECTION_DOWN) {
    set_sprite_tile(PLAYER_SPRITE, 1);
    if (last_pressed_horiz & DIRECTION_LEFT) {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX | S_FLIPY);
    } else {
      set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPY);
    }
  } else if (direction == (DIRECTION_LEFT | DIRECTION_UP)) {
    set_sprite_tile(PLAYER_SPRITE, 2);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == (DIRECTION_LEFT | DIRECTION_DOWN)) {
    set_sprite_tile(PLAYER_SPRITE, 3);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);
  } else if (direction == (DIRECTION_RIGHT | DIRECTION_UP)) {
    set_sprite_tile(PLAYER_SPRITE, 2);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  } else if (direction == (DIRECTION_RIGHT | DIRECTION_DOWN)) {
    set_sprite_tile(PLAYER_SPRITE, 3);
    set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP | S_FLIPX);
  }
}

uint8_t is_passable_tile(uint8_t tile) {
  // impassable tiles in our tilemap
  return tile != 0x1 && (tile < 184 || tile > 192);
}

uint8_t cur_string_char = 0;
uint8_t display_dialog = 1;

void update_aquaria(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(aquaria_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  // render_next_string_char(text_string, cur_string_char++, 0);
  if (display_dialog == 1) {
    cur_string_char = render_next_string_char_id(TEXT_STRING_TAKEOVER_WORLD, cur_string_char, 0);
  }

  int8_t move_x = 0;
  int8_t move_y = 0;
  prev_bg_pos_x = bg_pos_x;
  prev_bg_pos_y = bg_pos_y;

  last_direction = direction;
  direction = 0;

  if (input != 0) {
    display_dialog = 0;
    // Hide window
    move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
  }

  if (input & J_B) {
    show_song_note_sprites();
    last_note = cur_note;
    cur_note = 0;
    if (input & J_LEFT) {
      cur_note |= DIRECTION_LEFT;
    } else if (input & J_RIGHT) {
      cur_note |= DIRECTION_RIGHT;
    }
    if (input & J_UP) {
      cur_note |= DIRECTION_UP;
    } else if (input & J_DOWN) {
      cur_note |= DIRECTION_DOWN;
    }

    if (!(input & J_LEFT) && !(input & J_RIGHT) && !(input & J_UP) && !(input & J_DOWN)) {
      stop_note();
    } else {
      update_note();
    }
  } else {
    hide_song_note_sprites();
    stop_note();
    move_x = move_y = 0;
    if (input & J_LEFT) {
      uint16_t player_sprite_left_x = bg_pos_x + 76;
      uint16_t player_sprite_left_y = bg_pos_y + 72;
      // Check if we can move left
      if (player_sprite_left_x % 8 == 0) {
        uint8_t tile_left = get_aquaria_map_tile((player_sprite_left_x - 1) >> 3, (player_sprite_left_y - 4) >> 3);
        uint8_t tile_left2 = get_aquaria_map_tile((player_sprite_left_x - 1) >> 3, (player_sprite_left_y + 3) >> 3);
        if (is_passable_tile(tile_left) && is_passable_tile(tile_left2)) {
          bg_pos_x--;
          move_x = 1;
          moved_bg = 1;
        }
      } else {
        bg_pos_x--;
        move_x = 1;
        moved_bg = 1;
      }
      direction |= DIRECTION_LEFT;
      last_pressed_horiz = DIRECTION_LEFT;
    }
    if (input & J_RIGHT) {
      uint16_t player_sprite_right_x = bg_pos_x + 84;
      uint16_t player_sprite_right_y = bg_pos_y + 72;
      // Check if we can move right
      if (player_sprite_right_x % 8 == 0) {
        uint8_t tile_right = get_aquaria_map_tile((player_sprite_right_x + 1) >> 3, (player_sprite_right_y - 4) >> 3);
        uint8_t tile_right2 = get_aquaria_map_tile((player_sprite_right_x + 1) >> 3, (player_sprite_right_y + 3) >> 3);
        if (is_passable_tile(tile_right) && is_passable_tile(tile_right2)) {
          bg_pos_x++;
          move_x = -1;
          moved_bg = 1;
        }
      } else {
        bg_pos_x++;
        move_x = -1;
        moved_bg = 1;
      }
      direction |= DIRECTION_RIGHT;
      last_pressed_horiz = DIRECTION_RIGHT;
    }
    if (input & J_UP) {
      uint16_t player_sprite_up_x = bg_pos_x + 80;
      uint16_t player_sprite_up_y = bg_pos_y + 68;
      // Check if we can move up
      if (player_sprite_up_y % 8 == 0) {
        uint8_t tile_up = get_aquaria_map_tile((player_sprite_up_x - 4) >> 3, (player_sprite_up_y - 1) >> 3);
        uint8_t tile_up2 = get_aquaria_map_tile((player_sprite_up_x + 3) >> 3, (player_sprite_up_y - 1) >> 3);
        if (is_passable_tile(tile_up) && is_passable_tile(tile_up2)) {
          bg_pos_y--;
          move_y = 1;
          moved_bg = 1;
        }
      } else {
        bg_pos_y--;
        move_y = 1;
        moved_bg = 1;
      }
      direction |= DIRECTION_UP;
    }
    if (input & J_DOWN) {
      uint16_t player_sprite_down_x = bg_pos_x + 80;
      uint16_t player_sprite_down_y = bg_pos_y + 76;
      // Check if we can move down
      if (player_sprite_down_y % 8 == 0) {
        uint8_t tile_down = get_aquaria_map_tile((player_sprite_down_x - 4) >> 3, (player_sprite_down_y + 1) >> 3);
        uint8_t tile_down2 = get_aquaria_map_tile((player_sprite_down_x + 3) >> 3, (player_sprite_down_y + 1) >> 3);
        if (is_passable_tile(tile_down) && is_passable_tile(tile_down2)) {
          bg_pos_y++;
          move_y = -1;
          moved_bg = 1;
        }
      } else {
        bg_pos_y++;
        move_y = -1;
        moved_bg = 1;
      }
      direction |= DIRECTION_DOWN;
    }
  }

  update_sprite_positions(move_x, move_y);
  if (moved_bg) {
    move_bkg(bg_pos_x & 0xFF, bg_pos_y & 0xFF);

    prev_col = prev_bg_pos_x >> 3;
    prev_row = prev_bg_pos_y >> 3;
    cur_col = bg_pos_x >> 3;
    cur_row = bg_pos_y >> 3;

    if (cur_col > prev_col) {
      set_aquaria_map_tile_column(cur_col + 20, cur_row, cur_col + 20);
      set_aquaria_map_attrib_column(cur_col + 20, cur_row, cur_col + 20);
    } else if (cur_col < prev_col) {
      set_aquaria_map_tile_column(cur_col, cur_row, cur_col);
      set_aquaria_map_attrib_column(cur_col, cur_row, cur_col);
    }

    if (cur_row > prev_row) {
      set_aquaria_map_tile_row(cur_row + 18, cur_row + 18, cur_col);
      set_aquaria_map_attrib_row(cur_row + 18, cur_row + 18, cur_col);
    } else if (cur_row < prev_row) {
      set_aquaria_map_tile_row(cur_row, cur_row, cur_col);
      set_aquaria_map_attrib_row(cur_row, cur_row, cur_col);
    }

    if(last_direction != direction) {
      update_player_sprite();
    }
  }
}
