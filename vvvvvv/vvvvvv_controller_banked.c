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
#define PLAYER_DEAD        12
#define PLAYER_DEAD_TIMER  100

#define START_MINIMAP_TILES_IDX 94

#define MINIMAP_BLINK_AMOUNT 20

extern uint16_t curScreenX;
extern uint16_t curScreenY;
uint16_t lastScreenX;
uint16_t lastScreenY;
extern int16_t playerSpriteX;
extern int16_t playerSpriteY;
uint16_t lastPlayerSpriteX;
uint16_t lastPlayerSpriteY;
extern uint8_t playerFlipped;
uint8_t lastPlayerFlipped;
extern uint8_t playerMoveLeft;
extern uint8_t playerCanFlip;
extern uint8_t playerMoveAnimDelay;
extern uint8_t playerAnimApplied;
extern uint8_t isOnGround;
extern uint8_t playerHasGlasses;
uint8_t playerDead;
uint8_t playerDeadCountdown;
uint8_t vertFlipped = 0;

void save_game(void) BANKED {
  // hUGE_dosound(SFX_SAVEPOINT);
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
  // Play death sound
  // hUGE_dosound(SFX_DEATH);
  // Reset player position
  // playerSpriteX = 80 - 4;
  // playerSpriteY = 72 - 8;
  // playerFlipped = playerCanFlip = playerMoveLeft = 0;
  set_sprite_tile(PLAYER_SPRITE, PLAYER_DEAD);
  // Turn red
  set_sprite_prop(PLAYER_SPRITE, playerFlipped | playerMoveLeft | 0x2);
  playerDead = 1;
  playerDeadCountdown = 0;
  // move_sprite(PLAYER_SPRITE, playerSpriteX+8, playerSpriteY+16);
}

uint8_t is_vvvvvv_passable_tile(uint8_t tile) BANKED {
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

void check_tile_collisions(void) BANKED {
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
      // hUGE_dosound(SFX_FLIP);
    }
  } else {
    vertFlipped = 0;
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

  check_tile_collisions();
}
