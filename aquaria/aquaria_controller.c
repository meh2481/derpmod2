#include <gb/gb.h>
#include <rand.h>
#include "../utils/hUGEHelpers.h"
#include "tileset.h"
#include "map1_tiles.h"
#include "../utils/utils.h"
#include "../sprite/naija_sprites.h"
#include "../sfx/sfx.h"

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
#define DEFAULT_PLAYER_SPRITE_PROP  0x00
#define MAX_NOTE_SEQUENCE_LENGTH    8
#define NOTE_FRAMES_LENGTH          0x7F

uint16_t bg_pos_x = 0;
uint16_t bg_pos_y = 0;
uint8_t direction = 0;
uint8_t last_direction = 0;
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
uint8_t bulb_colors[] = {0,0,0};

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

  // Create player sprite
  set_sprite_tile(PLAYER_SPRITE, 0);
  set_sprite_prop(PLAYER_SPRITE, DEFAULT_PLAYER_SPRITE_PROP);

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
    move_sprite(
      sprite_num,
      SCREEN_WIDTH - (((bg_pos_x_tile - bulb_x + 20) << 3) + (bg_pos_x & 0b111)) + 8 + move_x,
      SCREEN_HEIGHT - (((bg_pos_y_tile - bulb_y + 18) << 3) + (bg_pos_y & 0b111)) + 16 + move_y
    );
  } else {
    // Hide the sprite if it's off the screen
    hide_sprite(sprite_num);
  }
}

void update_sprite_positions(int8_t move_x, int8_t move_y) {
  // If the note bulbs are on the screen, move them with the background
  if (bulbs_active & NOTE_BULB_1) {
    update_bulb_sprite(9, move_x, move_y, NOTE_BULB_X_1, NOTE_BULB_Y_1);
  }
  if (bulbs_active & NOTE_BULB_2) {
    update_bulb_sprite(10, move_x, move_y, NOTE_BULB_X_2, NOTE_BULB_Y_2);
  }
  if (bulbs_active & NOTE_BULB_3) {
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
  // Hide window
  move_win(WIN_X_OFFSET, SCREEN_HEIGHT);
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

void update_aquaria(uint8_t input) NONBANKED {
  // Switch to title music bank to update music
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(aquaria_music));
  hUGE_dosound();
  SWITCH_ROM(previous_bank);

  uint8_t moved_bg = 0;
  int8_t move_x = 0;
  int8_t move_y = 0;
  prev_bg_pos_x = bg_pos_x;
  prev_bg_pos_y = bg_pos_y;

  last_direction = direction;
  direction = 0;

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
      bg_pos_x--;
      move_x = 1;
      moved_bg = 1;
      direction |= DIRECTION_LEFT;
      last_pressed_horiz = DIRECTION_LEFT;
    }
    if (input & J_RIGHT) {
      bg_pos_x++;
      move_x = -1;
      moved_bg = 1;
      direction |= DIRECTION_RIGHT;
      last_pressed_horiz = DIRECTION_RIGHT;
    }
    if (input & J_UP) {
      bg_pos_y--;
      move_y = 1;
      moved_bg = 1;
      direction |= DIRECTION_UP;
    }
    if (input & J_DOWN) {
      bg_pos_y++;
      move_y = -1;
      moved_bg = 1;
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
