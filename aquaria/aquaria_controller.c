#include <gb/gb.h>
#include <gb/gbdecompress.h>
#include <gb/cgb.h>
#include "tileset.h"
#include "map1_tiles.h"

void init_aquaria(void) {

  gb_decompress_bkg_data(0, tileset);

  // Set up the palette
  set_bkg_palette(0, 6, tilesetCGBPal);

  // Set up the background tiles
  gb_decompress_bkg_data(0, tileset);

  // Draw the background
  VBK_REG = VBK_TILES;
  set_bkg_tiles(0, 0, 20, 18, map1_tilesPLN0);
  VBK_REG = VBK_ATTRIBUTES;
  set_bkg_tiles(0, 0, 20, 18, map1_tilesPLN1);
  VBK_REG = VBK_TILES;
}

void update_aquaria(void) {
  // Do nothing for now
}
