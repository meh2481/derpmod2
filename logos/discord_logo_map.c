/*

 DISCORD_LOGO_MAP.C

 Map Source File.

 Info:
   Section       :
   Bank          : 0
   Map size      : 20 x 12
   Tile set      : discord_logo.gbm.tiles.gbr
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

#define discord_logo_mapWidth 20
#define discord_logo_mapHeight 12

#define discord_logo_map discord_logo_mapPLN0
const unsigned char discord_logo_mapPLN0[] =
{
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
  0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,
  0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,
  0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
  0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,
  0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,
  0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,
  0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
  0x50,0x28,0x28,0x28,0x28,0x28,0x28,0x51,0x52,0x53,
  0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x28,0x5C,
  0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,
  0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
  0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,
  0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,
  0x28,0x28,0x28,0x28,0x28,0x85,0x28,0x28,0x28,0x28,
  0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,
  0x28,0x28,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,
  0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x28,0x28,0x28,
  0x28,0x28,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,
  0x9D,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0x28,0x28,0x28,
  0x28,0x28,0x28,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,
  0xAB,0xAC,0xAD,0xAE,0xAF,0x28,0x28,0x28,0x28,0x28,
  0x28,0x28,0x28,0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,
  0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0x28,0x28,0x28,0x28
};

void draw_discord_logo_bg(uint8_t tile_x, uint8_t tile_y) BANKED {
  // Draw the whole screen in tile 189, palette 0
  VBK_REG = VBK_TILES;
  init_bkg(189);
  VBK_REG = VBK_ATTRIBUTES;
  init_bkg(0);
  VBK_REG = VBK_TILES;
  // This is all palette 0 already too
  set_bkg_tiles(tile_x, tile_y, discord_logo_mapWidth, discord_logo_mapHeight, discord_logo_mapPLN0);
}

/* End of DISCORD_LOGO_MAP.C */
