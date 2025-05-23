/*

 SWORDFISH_STEAK_MAP.C

 Map Source File.

 Info:
   Section       :
   Bank          : 0
   Map size      : 8 x 7
   Tile set      : swordfish_steak.gbm.tiles.gbr
   Plane count   : 1 plane (8 bits)
   Plane order   : Planes are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/
#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>

#pragma bank 255

#define swordfish_steak_mapWidth 8
#define swordfish_steak_mapHeight 7
#define swordfish_steak_mapBank 0

const unsigned char swordfish_steak_mapPLN0[] =
{
  0x00,0x01,0x02,0x03,0x04,0x00,0x00,0x00,0x05,0x06,
  0x07,0x08,0x09,0x0A,0x00,0x00,0x0B,0x0C,0x0D,0x0E,
  0x0F,0x10,0x11,0x00,0x12,0x13,0x14,0x15,0x16,0x17,
  0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
  0x00,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x00,0x00,
  0x29,0x2A,0x2B,0x2C,0x2D,0x2E
};

void draw_swordfish_steak_bg(uint8_t tile_x, uint8_t tile_y) BANKED {
  VBK_REG = VBK_TILES;
  init_bkg(0);
  VBK_REG = VBK_ATTRIBUTES;
  init_bkg(0);
  VBK_REG = VBK_TILES;
  // This is all palette 0 already too
  set_bkg_tiles(tile_x, tile_y, swordfish_steak_mapWidth, swordfish_steak_mapHeight, swordfish_steak_mapPLN0);
}

/* End of SWORDFISH_STEAK_MAP.C */
