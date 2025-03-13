#pragma bank 255
#include <gb/gb.h>
#include <rand.h>
#include "../utils/hUGEHelpers.h"
#include "vvvvvv_tiles.h"
#include "vvvvvv_map.h"
#include "vvvvvv_palettes.h"
#include "sprite_tileset_tiles.h"
#include "../utils/utils.h"
#include "../sfx/sfx.h"
#include "../font/font_tiles.h"
#include "minimap_tiles.h"
#include "vvvvvv_controller.h"

#define NUM_SCREENS_X      7
#define NUM_SCREENS_Y      7
#define HALF_NUM_SCREENS_Y 4

#define PALETTE_FLIPLINES  1
#define PALETTE_SAVEPOINTS 2
#define PALETTE_MINIMAP    3
#define PALETTE_MAP_FOG    4

#define PLAYER_SPRITE      0
#define FALL_AMOUNT        4
#define PLAYER_MOVE_SPEED  2
#define PLAYER_ANIM_COUNT  4

#define WINDOW_MAP_X       95
#define WINDOW_MAP_Y       72

#define BLANK_MAP_TILE     91
#define MAP_SPIKE_TILE1    92
#define MAP_SPIKE_TILE2    93
#define MAP_SAVEPOINT_TILE 94
#define MAP_FLIP_VERT_TILE 95
#define MAP_FLIP_HORIZ_TILE 96
#define MAP_SIGN_TILE      97
#define PLAYER_DEAD        12
#define PLAYER_DEAD_TIMER  100

#define START_MINIMAP_TILES_IDX 94

#define MINIMAP_BLINK_AMOUNT 20

#define B_BLINK_DELAY      40

#define SPRITE_ANIM_DELAY  12
#define DISCO_BALL_ANIM_DELAY 25
#define DISCO_BALL_FLIP    32
#define DISCO_BALL         0


const uint8_t no_tiles[] = {
  91, 91
};

extern uint16_t curScreenX;
extern uint16_t curScreenY;
extern uint16_t lastScreenX;
extern uint16_t lastScreenY;
extern int16_t playerSpriteX;
extern int16_t playerSpriteY;
extern uint16_t lastPlayerSpriteX;
extern uint16_t lastPlayerSpriteY;
extern uint8_t playerFlipped;
extern uint8_t lastPlayerFlipped;
extern uint8_t playerMoveLeft;
extern uint8_t playerCanFlip;
extern uint8_t playerMoveAnimDelay;
extern uint8_t playerAnimApplied;
extern uint8_t isOnGround;
extern uint8_t playerHasGlasses;
uint8_t playerDead;
uint8_t playerDeadCountdown;
uint8_t vertFlipped = 0;
uint8_t horizFlipped = 0;
extern uint8_t display_dialog;
extern int8_t cur_displaying_string_char;
extern uint8_t cur_vvvvvv_dialogue;
extern uint8_t cur_vvvvvv_dialogue_start;
extern uint8_t cur_vvvvvv_dialogue_length;
extern uint8_t playerPressingA;
uint8_t hasDoneIntro = 0;
uint8_t b_button_blink = 0;
uint8_t b_button_frame = 26;
int16_t moveSprite1PosX = 0;
int16_t moveSprite1PosY = 0;
int16_t moveSprite2PosX = 0;
int16_t moveSprite2PosY = 0;
int16_t moveSprite1VelX = 0;
int16_t moveSprite1VelY = 0;
int16_t moveSprite2VelX = 0;
int16_t moveSprite2VelY = 0;
int16_t moveSprite3PosX = 0;
int16_t moveSprite3PosY = 0;
int16_t moveSprite3VelX = 0;
int16_t moveSprite3VelY = 0;
uint8_t seenLiString = 0;
uint8_t spriteAnimDelay = 0;
uint8_t spriteAnimFrame = 0;
uint8_t discoBallAnimDelay = 0;
uint8_t discoBallAnimFrame = 0;
uint8_t discoSparkleAnimFrame = 0;
uint8_t coolTextHue = 0;

void save_game(void) BANKED {
  //TODO hUGE_dosound(SFX_SAVEPOINT);
  if (isOnGround) {
    lastPlayerFlipped = playerFlipped;
    lastPlayerSpriteX = playerSpriteX;
    lastPlayerSpriteY = playerSpriteY;
    lastScreenX = curScreenX;
    lastScreenY = curScreenY;
  }
}

void player_respawn(void) BANKED {
  playerDead = 0;
  playerDeadCountdown = 0;
  playerSpriteX = lastPlayerSpriteX;
  playerSpriteY = lastPlayerSpriteY;
  playerFlipped = lastPlayerFlipped;
  set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft);
  move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
  set_sprite_tile(PLAYER_SPRITE, 0);
  curScreenX = lastScreenX;
  curScreenY = lastScreenY;
  vertFlipped = 0;
  draw_screen();
}

void player_die(void) BANKED {
  // TODO Play death sound
  // hUGE_dosound(SFX_DEATH);
  set_sprite_tile(PLAYER_SPRITE, PLAYER_DEAD);
  // Turn red
  set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft | 0x2);
  playerDead = 1;
  playerDeadCountdown = 0;
}

uint8_t is_vvvvvv_passable_tile(uint8_t tile) BANKED {
  if (playerHasGlasses && curScreenX == 0 && curScreenY == 3 && tile < 13) {
    // Door is open if the player is cool enough (collision, not visually)
    return 1;
  }
  // impassable tiles in our tilemap
  return tile > 90;
}

void fall_down(void) BANKED {
  playerSpriteY += FALL_AMOUNT;
  isOnGround = 0;

  if(playerSpriteY > SCREEN_HEIGHT) {
    // Player fell off the screen, reset to top
    playerSpriteY -= SCREEN_HEIGHT + 16;
    if (curScreenY == NUM_SCREENS_Y - 1) {
      curScreenY = 0;
    } else {
      curScreenY++;
    }
    draw_screen();
  }
  move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
}

void fall_up(void) BANKED {
  playerSpriteY -= FALL_AMOUNT;
  if(playerSpriteY <= -16) {
    // Player fell off top of screen, reset to bottom
    playerSpriteY += SCREEN_HEIGHT + 16;
    if (curScreenY == 0) {
      curScreenY = NUM_SCREENS_Y - 1;
    } else {
      curScreenY--;
    }
    draw_screen();
  }
  move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
  isOnGround = 0;
}

uint8_t map_tile;
uint8_t map_tile2;
uint8_t map_tile3;
uint8_t map_tile4;
uint8_t map_tile5;
uint8_t map_tile6;

void check_tile_collisions(uint8_t input) BANKED {
  // Check to see if we're hitting
  map_tile = map_tile2 = map_tile3 = map_tile4 = map_tile5 = map_tile6 = BLANK_MAP_TILE;
  if (curScreenY > 3) {
    map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY / 8));
    map_tile2 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY + 8) / 8);
    if (playerSpriteX % 8 != 0) {
      map_tile3 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX + 8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY / 8));
      map_tile4 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX + 8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY + 8) / 8);
    }
    if (playerSpriteY % 8 != 0) {
      map_tile5 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
      if (playerSpriteX % 8 != 0) {
        map_tile6 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX + 8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
      }
    }
  } else {
    map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY / 8));
    map_tile2 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY + 8) / 8);
    if (playerSpriteX % 8 != 0) {
      map_tile3 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY) / 8);
      map_tile4 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+8) / 8);
    }
    if (playerSpriteY % 8 != 0) {
      map_tile5 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX / 8), curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
      if (playerSpriteX % 8 != 0) {
        map_tile6 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
      }
    }
  }

  if (map_tile == MAP_SPIKE_TILE1 || map_tile2 == MAP_SPIKE_TILE1 || map_tile3 == MAP_SPIKE_TILE1 || map_tile4 == MAP_SPIKE_TILE1 || map_tile5 == MAP_SPIKE_TILE1 || map_tile6 == MAP_SPIKE_TILE1 ||
    map_tile == MAP_SPIKE_TILE2 || map_tile2 == MAP_SPIKE_TILE2 || map_tile3 == MAP_SPIKE_TILE2 || map_tile4 == MAP_SPIKE_TILE2 || map_tile5 == MAP_SPIKE_TILE2 || map_tile6 == MAP_SPIKE_TILE2) {
    // Player hit a spike
    player_die();
    return; // Stop testing
  }

  if (map_tile == MAP_SAVEPOINT_TILE || map_tile2 == MAP_SAVEPOINT_TILE || map_tile3 == MAP_SAVEPOINT_TILE || map_tile4 == MAP_SAVEPOINT_TILE || map_tile5 == MAP_SAVEPOINT_TILE || map_tile6 == MAP_SAVEPOINT_TILE) {
    // Player hit a savepoint
    save_game();
  }

  if (map_tile == MAP_FLIP_VERT_TILE || map_tile2 == MAP_FLIP_VERT_TILE || map_tile3 == MAP_FLIP_VERT_TILE || map_tile4 == MAP_FLIP_VERT_TILE || map_tile5 == MAP_FLIP_VERT_TILE || map_tile6 == MAP_FLIP_VERT_TILE) {
    // Player hit a flip line
    if (!vertFlipped && ((playerSpriteX % 8 >= 4 && !playerMoveLeft) || (playerSpriteX % 8 < 4 && playerMoveLeft))) {
      vertFlipped = 1;
      playerFlipped = playerFlipped ? 0 : S_FLIPY;
      set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft);
      // TODO hUGE_dosound(SFX_FLIP);
    }
  } else {
    vertFlipped = 0;
  }

  if (map_tile == MAP_FLIP_HORIZ_TILE || map_tile2 == MAP_FLIP_HORIZ_TILE || map_tile3 == MAP_FLIP_HORIZ_TILE || map_tile4 == MAP_FLIP_HORIZ_TILE || map_tile5 == MAP_FLIP_HORIZ_TILE || map_tile6 == MAP_FLIP_HORIZ_TILE) {
    // Player hit a flip line
    if (!horizFlipped && ((playerSpriteY % 8 >= 4 && !playerFlipped) || (playerSpriteY % 8 < 4 && playerFlipped))) {
      horizFlipped = 1;
      playerCanFlip = 0;
      if (!playerFlipped) {
        playerFlipped = S_FLIPY;
        set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft);
      } else {
        playerFlipped = 0;
        playerSpriteY += 4;
        move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
        set_sprite_prop(PLAYER_SPRITE, playerMoveLeft);
      }
      // TODO hUGE_dosound(SFX_FLIP);
    }
  } else {
    horizFlipped = 0;
  }

  if (curScreenX == 0 && curScreenY == 3) {
    if (map_tile == MAP_SIGN_TILE || map_tile2 == MAP_SIGN_TILE || map_tile3 == MAP_SIGN_TILE || map_tile4 == MAP_SIGN_TILE || map_tile5 == MAP_SIGN_TILE || map_tile6 == MAP_SIGN_TILE) {
      if (input & J_B) {
        // Player hit a sign
        display_dialog = 1;
        cur_displaying_string_char = 0;
        if (playerHasGlasses) {
          cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_CLUB_SIGN_2;
        } else {
          cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_CLUB_SIGN;
        }
        cur_vvvvvv_dialogue_length = 4;
        render_textbox_id(cur_vvvvvv_dialogue, 0);
      }
      // Show button prompt above player
      set_sprite_prop(PLAYER_SPRITE+1, 1);
      move_sprite(PLAYER_SPRITE+1, playerSpriteX+8, playerSpriteY+5);
      if (++b_button_blink >= B_BLINK_DELAY) {
        b_button_blink = 0;
        b_button_frame = b_button_frame == 26 ? 28 : 26;
        set_sprite_tile(PLAYER_SPRITE+1, b_button_frame);
      }
    } else {
      // Hide button prompt
      hide_sprite(PLAYER_SPRITE+1);
      b_button_blink = 0;
    }
  }

}

void update_player(uint8_t input) BANKED {
  if (playerDead) {
    if (playerDeadCountdown++ >= PLAYER_DEAD_TIMER) {
      player_respawn();
    } else {
      return;
    }
  }

  if (!playerFlipped) {
    // Check collisions with tiles below the player
    if (playerSpriteY % 8 == 0 && playerSpriteY <= 128) {
      if (curScreenY > 3) {
        if(curScreenX == 0 && playerSpriteX < 0) {
          // Read the next tile over
          map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        } else {
          map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + playerSpriteX / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
        if (curScreenX == NUM_SCREENS_X - 1 && playerSpriteX > 152) {
          map_tile2 = map_tile;
        } else {
          map_tile2 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      } else {
        if(curScreenX == 0 && playerSpriteX < 0) {
          map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        } else {
          map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + playerSpriteX / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
        if (curScreenX == NUM_SCREENS_X - 1 && playerSpriteX > 152) {
          map_tile2 = map_tile;
        } else {
          map_tile2 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      }
      if (is_vvvvvv_passable_tile(map_tile) && (playerSpriteX % 8 == 0 || is_vvvvvv_passable_tile(map_tile2))) {
        // Fall down
        fall_down();
      } else {
        // Stop falling
        isOnGround = 1;

        // The first time we land on the ground, start the VVVVVV intro sequence
        #ifndef DEBUG
        if (!hasDoneIntro) {
          hasDoneIntro = 1;
          // Display dialog and show text box
          display_dialog = 1;
          cur_displaying_string_char = 0;
          cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_WARP_SUCCESS;
          cur_vvvvvv_dialogue_length = 8;
          render_textbox_id(cur_vvvvvv_dialogue, 0);
        }
        #endif
      }
    } else {
      // Fall down
      fall_down();
    }
  } else {
    // Check collisions with tiles above the player
    if (playerSpriteY % 8 == 0 && playerSpriteY >= 8) {
      if (curScreenY > 3) {
        if(curScreenX == 0 && playerSpriteX < 0) {
          map_tile = get_vvvvvv_map_tile2(playerSpriteX / 8 + 1, (curScreenY-4) * SCREEN_HEIGHT_TILES + playerSpriteY / 8 - 1);
        } else {
          map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + playerSpriteX / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY - 8) / 8);
        }
        if (curScreenX == NUM_SCREENS_X - 1 && playerSpriteX > 152) {
          map_tile2 = map_tile;
        } else {
          map_tile2 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY - 8) / 8);
        }
      } else {
        if(curScreenX == 0 && playerSpriteX < 0) {
          map_tile = get_vvvvvv_map_tile((playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY - 8) / 8);
        } else {
          map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + playerSpriteX / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY - 8) / 8);
        }
        if (curScreenX == NUM_SCREENS_X - 1 && playerSpriteX > 152) {
          map_tile2 = map_tile;
        } else {
          map_tile2 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY - 8) / 8);
        }
      }
      if (is_vvvvvv_passable_tile(map_tile) && (playerSpriteX % 8 == 0 || is_vvvvvv_passable_tile(map_tile2))) {
        // Fall up
        fall_up();
      } else {
        // Stop falling
        isOnGround = 1;
      }
    } else {
      // Fall up
      fall_up();
    }
  }

  // Flip player when pressing button
  if(input & J_A && playerCanFlip && isOnGround) {
    isOnGround = 0;
    playerCanFlip = 0;
    playerPressingA = 1;
    if (!playerFlipped) {
      playerFlipped = S_FLIPY;
      set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft);
    } else {
      playerFlipped = 0;
      set_sprite_prop(PLAYER_SPRITE, playerMoveLeft);
    }
  } else if (!(input & J_A) && isOnGround) {
    playerCanFlip = 1;
  }

  // Reset map tiles
  map_tile = map_tile2 = map_tile3 = BLANK_MAP_TILE;

  // Move left and right
  if(input & J_LEFT) {
    playerMoveLeft = S_FLIPX;
    set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft);
    // Check tiles to the left of the player
    if(curScreenX > 0 || playerSpriteX >= 8) {
      if (curScreenY > 3) {
        map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + playerSpriteY / 8);
        map_tile2 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+8) / 8);
        if (playerSpriteY % 8 != 0) {
          map_tile3 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      } else {
        map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, curScreenY * SCREEN_HEIGHT_TILES + playerSpriteY / 8);
        map_tile2 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+8) / 8);
        if (playerSpriteY % 8 != 0) {
          map_tile3 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX-2) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      }
    }
    if (is_vvvvvv_passable_tile(map_tile) && is_vvvvvv_passable_tile(map_tile2) && is_vvvvvv_passable_tile(map_tile3)) {
      playerSpriteX -= PLAYER_MOVE_SPEED;
      if (playerSpriteX < -8) {
        playerSpriteX += SCREEN_WIDTH + 8;
        if (curScreenX == 0) {
          curScreenX = NUM_SCREENS_X - 1;
        } else {
          curScreenX--;
        }
        draw_screen();
      }
    }
    move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
  } else if(input & J_RIGHT) {
    playerMoveLeft = 0;
    set_sprite_prop(PLAYER_SPRITE, playerFlipped);
    // Check tiles to the right of the player
    if(curScreenX < NUM_SCREENS_X - 1 || playerSpriteX <= 152) {
      if (curScreenY > 3) {
        map_tile = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + playerSpriteY / 8);
        map_tile2 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+8) / 8);
        if (playerSpriteY % 8 != 0) {
          map_tile3 = get_vvvvvv_map_tile2(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, (curScreenY-4) * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      } else {
        map_tile = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + playerSpriteY / 8);
        map_tile2 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+8) / 8);
        if (playerSpriteY % 8 != 0) {
          map_tile3 = get_vvvvvv_map_tile(curScreenX * SCREEN_WIDTH_TILES + (playerSpriteX+8) / 8, curScreenY * SCREEN_HEIGHT_TILES + (playerSpriteY+16) / 8);
        }
      }
    }
    if (is_vvvvvv_passable_tile(map_tile) && is_vvvvvv_passable_tile(map_tile2) && is_vvvvvv_passable_tile(map_tile3)) {
      playerSpriteX += PLAYER_MOVE_SPEED;
      if (playerSpriteX > SCREEN_WIDTH) {
        playerSpriteX -= SCREEN_WIDTH + 8;
        if (curScreenX == NUM_SCREENS_X - 1) {
          curScreenX = 0;
        } else {
          curScreenX++;
        }
        draw_screen();
      }
    }
    move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
  } else {
    // Stop player anim
    set_sprite_tile(PLAYER_SPRITE, playerHasGlasses);
    playerMoveAnimDelay = 0;
    playerAnimApplied = 0;
  }
  if (isOnGround && (input & (J_LEFT | J_RIGHT))) {
    // Anim player walking
    playerMoveAnimDelay++;
    if (playerMoveAnimDelay > PLAYER_ANIM_COUNT) {
      playerMoveAnimDelay = 0;
      if (playerAnimApplied) {
        playerAnimApplied = 0;
        set_sprite_tile(PLAYER_SPRITE, playerHasGlasses + 2);
      } else {
        playerAnimApplied = 1;
        set_sprite_tile(PLAYER_SPRITE, playerHasGlasses);
      }
    }
  } else {
    // Stop player anim
    set_sprite_tile(PLAYER_SPRITE, playerHasGlasses);
    playerMoveAnimDelay = 0;
    playerAnimApplied = 0;
  }

  check_tile_collisions(input);
  check_sprite_collisions();
}

void add_vvvvvv_sprites(uint8_t screenX, uint8_t screenY) BANKED {

  // Hide all sprites by default
  for (i = 1; i < 10; i++) {
    set_sprite_tile(i, 0);
    set_sprite_prop(i, 0);
    move_sprite(i, 0, 0);
  }

  if(screenX == 0 && screenY == 0) {
    // Add glasses sprite
    if (!playerHasGlasses) {
      set_sprite_tile(1, 14);
      set_sprite_prop(1, 0);
      move_sprite(1, 48+8, 40+16);
    }
  } else if (screenX == 0 && screenY == 3) {
    // Hide door tiles (visually, not collisions) if player has glasses
    if (playerHasGlasses) {
      set_bkg_tiles(15, 9, 2, 1, no_tiles);
      set_bkg_tiles(15, 10, 2, 1, no_tiles);
    }
    set_sprite_tile(PLAYER_SPRITE+1, 26);
  } else if (screenX == 1 && screenY == 5) {
    if (playerHasGlasses) {
      // Let the player know about the map feature if they're backtracking
      display_dialog = 1;
      cur_displaying_string_char = 0;
      cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_CHECK_MAP;
      cur_vvvvvv_dialogue_length = 1;
      render_textbox_id(cur_vvvvvv_dialogue, 0);
    }
  } else if (screenX == 2 && screenY == 5) {
    // Show "LI" 1 sprite
    set_sprite_tile(1, 16);
    set_sprite_tile(2, 18);
    set_sprite_prop(1, 1);
    set_sprite_prop(2, 1);
    moveSprite1PosX = 32;
    moveSprite1PosY = 40;
    moveSprite1VelX = 0;
    moveSprite1VelY = 3;
    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);
    move_sprite(2, moveSprite1PosX+16, moveSprite1PosY+16);

    // Show "LI" 2 sprite
    set_sprite_tile(3, 16);
    set_sprite_tile(4, 18);
    set_sprite_prop(3, 1);
    set_sprite_prop(4, 1);
    moveSprite2PosX = 112;
    moveSprite2PosY = 88;
    moveSprite2VelX = 0;
    moveSprite2VelY = -3;
    move_sprite(3, moveSprite2PosX+8, moveSprite2PosY+16);
    move_sprite(4, moveSprite2PosX+16, moveSprite2PosY+16);
  } else if (screenX == 1 && screenY == 4) {
    // Show mini "LI" sprites
    set_sprite_tile(1, 20);
    set_sprite_tile(2, 20);
    set_sprite_tile(3, 20);

    set_sprite_prop(1, 1);
    set_sprite_prop(2, 1);
    set_sprite_prop(3, 1);

    moveSprite1PosX = 64;
    moveSprite1PosY = 45;
    moveSprite1VelX = 0;
    moveSprite1VelY = 2;

    moveSprite2PosX = 96;
    moveSprite2PosY = 91;
    moveSprite2VelX = 0;
    moveSprite2VelY = -2;

    moveSprite3PosX = 128;
    moveSprite3PosY = 45;
    moveSprite3VelX = 0;
    moveSprite3VelY = 2;

    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);
    move_sprite(2, moveSprite2PosX+8, moveSprite2PosY+16);
    move_sprite(3, moveSprite3PosX+8, moveSprite3PosY+16);
  } else if (screenX == 2 && screenY == 1) {
    set_sprite_tile(1, 22);
    set_sprite_tile(2, 22);

    set_sprite_prop(1, 1 | S_FLIPX);
    set_sprite_prop(2, 1);

    moveSprite1PosX = 24;
    moveSprite1PosY = 68;
    moveSprite1VelX = 3;
    moveSprite1VelY = 0;

    moveSprite2PosX = 144;
    moveSprite2PosY = 84;
    moveSprite2VelX = -3;
    moveSprite2VelY = 0;

    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);
    move_sprite(2, moveSprite2PosX+8, moveSprite2PosY+16);
  }
}

uint8_t check_sprite_collided(uint8_t playerX, uint8_t playerY, uint8_t spriteX, uint8_t spriteY, uint8_t spriteWidth, uint8_t spriteHeight) BANKED {
  return playerX < spriteX + spriteWidth &&
    playerX + 8 > spriteX &&
    playerY < spriteY + spriteHeight &&
    playerY + 16 > spriteY;
}

uint8_t rval, bval, gval;

void check_sprite_collisions(void) BANKED {
  if(curScreenX == 0 && curScreenY == 0) {
    // Sprite on this screen is the glasses, at position 48, 40
    if (isOnGround && !playerHasGlasses && check_sprite_collided(playerSpriteX, playerSpriteY, 48, 40, 8, 8)) {
      // Player picked up the glasses
      playerHasGlasses = 4;
      // TODO hUGE_dosound(SFX_GETGLASSES);
      // Hide glasses sprite
      set_sprite_tile(1, 0);
      set_sprite_prop(1, 0);
      move_sprite(1, 0, 0);

      // Display dialog and show text box
      display_dialog = 1;
      cur_displaying_string_char = 0;
      cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_SUNGLASSES_GET;
      cur_vvvvvv_dialogue_length = 2;
      render_textbox_id(cur_vvvvvv_dialogue, 0);

      set_sprite_tile(PLAYER_SPRITE, playerHasGlasses);
      // Switch palette so there's no background stuff behind glasses and player mouth
      set_sprite_palette_entry(0, 3, RGB(0, 0, 0));
    }
  } else if (curScreenX == 2 && curScreenY == 5) {
    // Update move sprite 1
    moveSprite1PosX += moveSprite1VelX;
    moveSprite1PosY += moveSprite1VelY;
    if (moveSprite1PosY < 42 || moveSprite1PosY > 86) {
      moveSprite1VelY = -moveSprite1VelY;
    }
    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);
    move_sprite(2, moveSprite1PosX+16, moveSprite1PosY+16);

    // Update move sprite 2
    moveSprite2PosX += moveSprite2VelX;
    moveSprite2PosY += moveSprite2VelY;
    if (moveSprite2PosY < 42 || moveSprite2PosY > 86) {
      moveSprite2VelY = -moveSprite2VelY;
    }
    move_sprite(3, moveSprite2PosX+8, moveSprite2PosY+16);
    move_sprite(4, moveSprite2PosX+16, moveSprite2PosY+16);

    // Sprite on this screen are the "LI" sprites (slightly smaller than 16x16)
    if (check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite1PosX+3, moveSprite1PosY+3, 10, 10) ||
      check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite2PosX+3, moveSprite2PosY+3, 10, 10)) {
      // Player hit one of the "LI" sprites
      player_die();
    }

    if (!seenLiString && playerSpriteY < 16 && playerSpriteX < 40) {
      // Pause to make joke about li
      seenLiString = 1;

      // Flip player to face right
      playerMoveLeft = 0;
      set_sprite_prop(PLAYER_SPRITE, playerFlipped);

      // Display dialog and show text box
      display_dialog = 1;
      cur_displaying_string_char = 0;
      cur_vvvvvv_dialogue_start = cur_vvvvvv_dialogue = TEXT_STRING_SEEN_LI;
      cur_vvvvvv_dialogue_length = 1;
      render_textbox_id(cur_vvvvvv_dialogue, 0);
    }
  } else if (curScreenX == 1 && curScreenY == 4) {
    // Update move sprite 1
    moveSprite1PosX += moveSprite1VelX;
    moveSprite1PosY += moveSprite1VelY;
    if (moveSprite1PosY < 30 || moveSprite1PosY > 105) {
      moveSprite1VelY = -moveSprite1VelY;
    }
    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);

    // Update move sprite 2
    moveSprite2PosX += moveSprite2VelX;
    moveSprite2PosY += moveSprite2VelY;
    if (moveSprite2PosY < 30 || moveSprite2PosY > 105) {
      moveSprite2VelY = -moveSprite2VelY;
    }
    move_sprite(2, moveSprite2PosX+8, moveSprite2PosY+16);

    // Update move sprite 3
    moveSprite3PosX += moveSprite3VelX;
    moveSprite3PosY += moveSprite3VelY;
    if (moveSprite3PosY < 30 || moveSprite3PosY > 105) {
      moveSprite3VelY = -moveSprite3VelY;
    }
    move_sprite(3, moveSprite3PosX+8, moveSprite3PosY+16);

    // Sprite on this screen are the mini "LI" sprites
    if (check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite1PosX, moveSprite1PosY, 8, 8) ||
      check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite2PosX, moveSprite2PosY, 8, 8) ||
      check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite3PosX, moveSprite3PosY, 8, 8)) {
      // Player hit one of the mini "LI" sprites
      player_die();
    }
  } else if (curScreenX == 2 && curScreenY == 1) {
    // Update move sprite 1
    moveSprite1PosX += moveSprite1VelX;
    if (moveSprite1PosX < 24) {
      moveSprite1PosX = 24;
      moveSprite1VelX = 3;
      set_sprite_prop(1, 1 | S_FLIPX);
    }
    if (moveSprite1PosX > 144) {
      moveSprite1PosX = 144;
      moveSprite1VelX = -3;
      set_sprite_prop(1, 1);
    }
    move_sprite(1, moveSprite1PosX+8, moveSprite1PosY+16);

    // Update move sprite 2
    moveSprite2PosX += moveSprite2VelX;
    if (moveSprite2PosX < 24) {
      moveSprite2PosX = 24;
      moveSprite2VelX = 3;
      set_sprite_prop(2, 1 | S_FLIPX);
    }
    if (moveSprite2PosX > 144) {
      moveSprite2PosX = 144;
      moveSprite2VelX = -3;
      set_sprite_prop(2, 1);
    }
    move_sprite(2, moveSprite2PosX+8, moveSprite2PosY+16);

    // Sprite on this screen are the fish sprites
    if (check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite1PosX, moveSprite1PosY, 8, 8) ||
      check_sprite_collided(playerSpriteX, playerSpriteY, moveSprite2PosX, moveSprite2PosY, 8, 8)) {
      // Player hit one of the fish sprites
      player_die();
    }

    // Animate fish sprites
    if (++spriteAnimDelay >= SPRITE_ANIM_DELAY) {
      spriteAnimDelay = 0;
      if (spriteAnimFrame == 0) {
        spriteAnimFrame = 1;
        set_sprite_tile(1, 24);
        set_sprite_tile(2, 24);
      } else {
        spriteAnimFrame = 0;
        set_sprite_tile(1, 22);
        set_sprite_tile(2, 22);
      }
    }
  } else if (curScreenX == 1 && curScreenY == 3) {
    if (++discoBallAnimDelay >= DISCO_BALL_ANIM_DELAY) {
      // Animate disco ball bg tile
      discoBallAnimDelay = 0;
      if (discoBallAnimFrame == 0) {
        discoBallAnimFrame = 1;
        set_bkg_attribute_xy(10, 2, DISCO_BALL_FLIP);
      } else {
        discoBallAnimFrame = 0;
        set_bkg_attribute_xy(10, 2, DISCO_BALL);
      }

      // Animate disco ball sparkle
      if (++discoSparkleAnimFrame > 2) {
        discoSparkleAnimFrame = 0;
        set_bkg_palette_entry(5, 3, RGB(0, 0, 0));
      } else {
        set_bkg_palette_entry(5, discoSparkleAnimFrame, RGB(0, 0, 0));
      }
      set_bkg_palette_entry(5, discoSparkleAnimFrame+1, RGB(31, 31, 31));
    }

    // Every frame, update the hue of the cool text to create a rainbow effect
    coolTextHue += 1;
    if (coolTextHue >= 128) {
      coolTextHue = 0;
    }
    rval = coolTextHue < 32 ? 31 : coolTextHue < 64 ? 31 - (coolTextHue - 32) : coolTextHue < 96 ? 0 : coolTextHue - 96;
    gval = coolTextHue < 32 ? coolTextHue : coolTextHue < 64 ? 31 : coolTextHue < 96 ? 31 - (coolTextHue - 64) : 0;
    bval = coolTextHue < 32 ? 0 : coolTextHue < 64 ? coolTextHue - 32 : coolTextHue < 96 ? 31 : 31 - (coolTextHue - 96);
    set_bkg_palette_entry(7, 0, RGB(rval, gval, bval));
  }
}
