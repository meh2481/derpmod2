#pragma bank 0

#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include <stdlib.h>
#include "../utils/hUGEHelpers.h"
#include "../utils/utils.h"
#include "tileset.h"
#include "map1_tiles.h"

uint8_t* tmpPtr;
uint16_t i16;

void init_aquaria(void) {
  gamestate = STATE_PLAY;
  hUGE_init(&the_traveller);

  gb_decompress_bkg_data(0, tileset);

  // Set up the palette
  set_bkg_palette(0, 6, tilesetCGBPal);

  // Draw the background
  tmpPtr = (uint8_t*)malloc(20 * 18);
  for (i16 = 0; i16 < 20 * 18; i16++) {
    tmpPtr[i16] = map1_tilesPLN0[i16];
  }
  VBK_REG = VBK_TILES;
  set_bkg_tiles(0, 0, 20, 18, tmpPtr);
  VBK_REG = VBK_ATTRIBUTES;
  for (i16 = 0; i16 < 20 * 18; i16++) {
    tmpPtr[i16] = map1_tilesPLN1[i16] & 0b11110111;
  }
  set_bkg_tiles(0, 0, 20, 18, tmpPtr);
  VBK_REG = VBK_TILES;

  free(tmpPtr);
}

void update_aquaria(void) {
  // Do nothing for now
}
