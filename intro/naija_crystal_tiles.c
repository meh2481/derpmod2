/*

 NAIJA_CRYSTAL_TILES.C

 Tile Source File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : GB-Compress.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 175

  Palette colors       : Included.
  SGB Palette          : None.
  CGB Palette          : Constant per entry.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/
#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <gb/gbdecompress.h>

#pragma bank 255

/* Start of tile array. */
const unsigned char naija_crystal_tiles[] =
{
  0xEE,0x92,0xFF,0x55,0xFF,0x51,0xFF,0x55,
  0xFF,0x9E,0xFF,0xAF,0xFF,0x5F,0xFF,0x5F,
  0xFF,0x80,0xFF,0x5A,0xFF,0x40,0xFF,0x55,
  0xFF,0x00,0xFF,0xD0,0xFF,0xC9,0xFF,0xE0,
  0xFF,0x00,0xFF,0xA8,0xFF,0x02,0xFF,0x51,
  0xFF,0x00,0xFF,0x84,0xFF,0x20,0xFF,0x41,
  0x43,0xFF,0x00,0xC1,0xFF,0x40,0x84,0xF6,
  0xFF,0xC0,0x50,0x86,0xF0,0xFF,0xD8,0x01,
  0xFE,0x00,0xFF,0x03,0xFC,0x03,0xFD,0x07,
  0xF8,0x7B,0x80,0x3C,0xC0,0xFF,0x00,0xF5,
  0x80,0xFE,0x00,0xFD,0x40,0xFE,0x00,0xFB,
  0x84,0xD1,0xFF,0xCA,0x7F,0x00,0xFF,0x00,
  0xBF,0x00,0xBF,0x00,0xEF,0x00,0x5D,0x44,
  0x00,0xFF,0xED,0x00,0xE9,0x00,0xB6,0x00,
  0xDD,0x00,0xCB,0x34,0xE9,0x16,0xC7,0x38,
  0xF1,0x0E,0xE6,0x19,0x72,0x0D,0xEB,0x14,
  0x48,0x17,0xFF,0x00,0x95,0x15,0xEA,0x08,
  0xEA,0x0A,0xD7,0x10,0xD5,0x14,0xEA,0x02,
  0xEA,0x0A,0x9F,0xFF,0x6F,0xFF,0x57,0xFF,
  0xF7,0x02,0xFF,0x42,0x7F,0xFF,0xD0,0xE0,
  0xFF,0xF4,0xFF,0xE0,0xFF,0xF0,0xFF,0xF4,
  0xFF,0xE8,0xFF,0xF0,0xFF,0xF0,0xFF,0x04,
  0x88,0x74,0xFF,0xC0,0x00,0x84,0xF4,0xFF,
  0xC2,0x04,0xFF,0x01,0x86,0x5C,0xFF,0xD9,
  0x14,0xFF,0x01,0xFF,0x01,0xFF,0x17,0xFA,
  0x0F,0xF0,0x2F,0xF4,0x3F,0xE1,0x3F,0xC8,
  0xFF,0x40,0x7F,0x90,0xFF,0xEA,0xFD,0x00,
  0xFF,0x00,0x83,0xFC,0xFF,0x87,0xF8,0xFF,
  0xFF,0x77,0x00,0x9B,0x00,0x7F,0x00,0x5F,
  0x00,0xBB,0x00,0xDD,0x00,0x7F,0x00,0x5D,
  0x00,0x6B,0x00,0xFD,0x00,0x57,0x00,0xFD,
  0x00,0xAA,0x00,0xFF,0x00,0xD5,0x00,0xFF,
  0x00,0x76,0x09,0x71,0x0E,0x72,0x0D,0xB8,
  0x07,0xF9,0x06,0xBC,0x03,0x7E,0x01,0x5F,
  0x00,0xF5,0x01,0x55,0x95,0xF5,0x00,0x6B,
  0x8A,0x75,0x84,0x2A,0xC2,0xBD,0x44,0x23,
  0xC2,0xDE,0x7F,0xFF,0x1F,0xFF,0xDF,0xFF,
  0x4F,0xFF,0x2F,0xFF,0xC7,0xFF,0x4B,0xFF,
  0x11,0xFF,0xF8,0xFF,0xF0,0xFF,0xF8,0xFF,
  0xF4,0xFF,0xF8,0xFF,0xF9,0xFF,0xF8,0xFF,
  0xFE,0x86,0xD0,0xFE,0xC0,0x80,0x85,0xDA,
  0xFE,0xC9,0xFF,0x01,0xFF,0x27,0xFF,0x0F,
  0xFF,0xBF,0xFF,0x3F,0x05,0xFF,0xCA,0xFD,
  0xFF,0xF6,0xFF,0x5A,0xFF,0xFE,0xFF,0xFF,
  0xFF,0xFB,0x84,0xEF,0xFF,0xF1,0xF7,0xFF,
  0xFF,0xFB,0x80,0xFD,0xC0,0xF7,0x50,0xFD,
  0xE8,0xFB,0x68,0xFF,0xEC,0xF9,0xB8,0xFE,
  0xDE,0xDF,0x00,0x7F,0x00,0xAF,0x00,0xFF,
  0x00,0x5F,0x00,0xDB,0x00,0xFF,0x00,0xAF,
  0x00,0xEB,0x00,0x7D,0x00,0xF7,0x00,0xBE,
  0x00,0xF7,0x00,0xFF,0x00,0xF7,0x00,0x7D,
  0x84,0xE6,0xFF,0xFF,0xBF,0x00,0xDF,0x00,
  0x7E,0x01,0xBF,0x00,0xFA,0x05,0xFB,0x04,
  0x7D,0x80,0xBF,0x40,0xFF,0x00,0xFA,0x05,
  0xAB,0x54,0x6D,0x92,0xB6,0x49,0x5B,0xA4,
  0x44,0xFF,0x50,0xFF,0x24,0xFF,0xA2,0xFF,
  0x10,0xFF,0xA1,0xFF,0x10,0xFF,0x42,0xFF,
  0xB8,0xFF,0xDE,0xFF,0x6C,0xFF,0x36,0xFF,
  0x5B,0xFF,0x2F,0xFF,0x75,0xFF,0x1F,0xFF,
  0x83,0xFF,0x05,0xFF,0xFB,0x03,0xFF,0x03,
  0xFF,0x0F,0xFE,0x07,0xFD,0x8F,0xFF,0x8F,
  0xF9,0x7F,0xDD,0xFF,0xDE,0xFF,0x7F,0xFF,
  0x5D,0xFF,0xF7,0xFF,0x7A,0xFF,0xFB,0xFF,
  0x75,0xFF,0xDD,0xFF,0xFE,0xFF,0x5F,0xFF,
  0x7F,0xFF,0xBF,0xFF,0xD7,0xFF,0x7F,0xFF,
  0xAD,0xFE,0xF6,0xFF,0xFB,0xFF,0xBD,0xFF,
  0xED,0xFF,0x76,0xFF,0xDD,0xFF,0xB6,0xFF,
  0xF5,0x83,0x6C,0xFF,0xE3,0xF7,0x00,0xDD,
  0x80,0xB7,0x80,0xBE,0x80,0xAF,0x80,0xAB,
  0xA0,0xFF,0x00,0xEF,0x00,0xFF,0x00,0xFE,
  0x01,0xFC,0x03,0xFA,0x07,0xF9,0x07,0xE0,
  0x1F,0xE0,0x1F,0xAA,0x55,0x80,0x7F,0x01,
  0xFE,0x83,0xD0,0xFD,0xD6,0x04,0xFF,0x02,
  0xFF,0xAD,0x52,0xB6,0x49,0xAB,0x54,0x59,
  0xA6,0xAA,0x55,0x2A,0xD5,0x95,0x6A,0x55,
  0xAA,0x20,0xFF,0x88,0x84,0xAA,0xFD,0xD0,
  0xA2,0xFD,0x00,0xFF,0x25,0xFA,0x00,0xFF,
  0x37,0xFF,0x1F,0xFF,0x1F,0xFF,0x4F,0xFF,
  0x1F,0x42,0xFF,0x0F,0xD4,0xFF,0x2F,0xFF,
  0x97,0xE5,0xBF,0xEB,0xF7,0xC6,0xFF,0xD7,
  0xF7,0xA2,0xF7,0xD7,0xFB,0xD2,0xFF,0xFF,
  0xFF,0x5B,0x84,0xD0,0xFE,0xFD,0xBF,0xFF,
  0xDF,0xFF,0xF5,0xFF,0xBF,0xFF,0x5F,0xFF,
  0xAD,0xFF,0xDF,0xFF,0x7A,0xFF,0xBD,0xFF,
  0xD7,0x7F,0x7A,0xBF,0xAA,0xFF,0xAD,0xFF,
  0xB6,0xFD,0x55,0xFF,0xD5,0xDC,0x54,0xF6,
  0x54,0xB9,0xA8,0xD3,0xD0,0xDE,0x00,0xB7,
  0x80,0x6F,0x00,0xAF,0x02,0x7F,0x01,0xBC,
  0x07,0xFA,0x17,0xF1,0x1F,0xE4,0x1F,0x92,
  0x7F,0xC0,0xBF,0x10,0x86,0x44,0xFD,0xFF,
  0x02,0xFF,0x10,0xFF,0x05,0xFF,0x00,0xFF,
  0x6A,0xFF,0x00,0xFF,0xA1,0xFF,0x08,0xFF,
  0xC0,0xFF,0x15,0xEA,0x15,0xEA,0x0A,0xF5,
  0x8A,0xF5,0x05,0xFA,0x42,0xFD,0x0A,0xF5,
  0xA1,0xFE,0x15,0xEA,0x90,0xFF,0xAD,0x52,
  0x52,0xAD,0xFD,0x92,0xFE,0x41,0xFF,0x12,
  0xFF,0x40,0x47,0xBF,0x03,0xFF,0x47,0xBF,
  0x2F,0xD0,0xAF,0x51,0xFE,0x00,0xFF,0x40,
  0xFF,0xD7,0x10,0xFF,0xE9,0xFD,0x6D,0xFF,
  0xD5,0xFF,0x75,0xFF,0x56,0xBF,0x2A,0xAF,
  0x8A,0xF7,0x05,0xFF,0x69,0x2B,0x2A,0xD7,
  0x45,0xF7,0x75,0xAF,0xAD,0xFF,0xD6,0xFF,
  0xAD,0xFF,0x6F,0x7F,0x6D,0xEF,0xAA,0xFA,
  0x6A,0xFF,0x68,0xFF,0xD5,0xFF,0xF5,0xFF,
  0xAD,0xFF,0xF7,0x55,0x44,0xF7,0x80,0x3E,
  0x03,0xFD,0x0B,0x7A,0x2F,0xE1,0xBF,0xF4,
  0xFF,0xFF,0xF2,0xFF,0xE0,0xBF,0x8A,0xFF,
  0x50,0xFF,0x0A,0xFF,0x40,0xFF,0x28,0xFF,
  0x41,0xFF,0x95,0xFF,0x10,0xFF,0x89,0xFF,
  0x00,0xFF,0x15,0xFF,0x80,0xFF,0x2A,0xFF,
  0x80,0xFF,0x64,0xFF,0x24,0xFF,0x80,0xFF,
  0x4A,0xFF,0x00,0xFF,0x49,0xFF,0x00,0xFF,
  0x92,0xFF,0x00,0xFF,0x0A,0xFD,0x41,0xFE,
  0x00,0xFF,0x28,0xFF,0x05,0xFE,0x20,0xFF,
  0x22,0xFF,0xC1,0x10,0xFF,0x84,0x2B,0xFE,
  0xDD,0x28,0xAF,0x55,0xEB,0x54,0x39,0xD7,
  0x86,0x79,0xA2,0x5D,0xEF,0x00,0xEB,0x20,
  0xD7,0x00,0xD5,0x50,0xE7,0x00,0xF9,0x40,
  0xEA,0x00,0xEA,0xA0,0xFB,0x02,0xF5,0x83,
  0x70,0xFE,0xD8,0x00,0xF7,0x01,0xFD,0x00,
  0xFB,0x83,0x79,0x00,0xFF,0xBA,0xDF,0x5F,
  0xFF,0xBD,0xFF,0xAF,0xFF,0x7B,0xFF,0xAE,
  0xFB,0xDB,0x7F,0x6A,0x84,0xB8,0xFE,0xC0,
  0x7F,0x09,0xFF,0x83,0x4C,0xFD,0x83,0x6F,
  0xFD,0x85,0x5F,0xFD,0xD8,0xF7,0xFF,0x03,
  0xFF,0xA2,0xFF,0x0F,0xFF,0x8B,0xFF,0x3D,
  0xFF,0x16,0xFF,0x6A,0xFF,0x49,0xFF,0x51,
  0xFF,0xAA,0xFF,0x40,0xFF,0x29,0x86,0xE8,
  0xFB,0xCC,0xA1,0xFF,0x24,0xFF,0x02,0xFF,
  0x20,0xFF,0x09,0xFF,0x00,0xFF,0xA5,0x84,
  0x5A,0xFF,0xF8,0x02,0xFF,0x21,0xFF,0x08,
  0xFF,0x82,0xFF,0x60,0xFF,0x08,0xFF,0x02,
  0xFF,0x40,0xFF,0x90,0x7F,0xE4,0x9B,0xA5,
  0x5E,0xD0,0x2F,0xF0,0x0F,0xFC,0x43,0xF8,
  0x0F,0xFE,0x01,0xEA,0x00,0x74,0xC0,0x6A,
  0x80,0x75,0xC0,0x69,0x80,0x75,0xC0,0x6A,
  0x80,0x2A,0xE0,0xFF,0x83,0x72,0x00,0xFF,
  0x01,0x7D,0x00,0x77,0x84,0x34,0xFD,0xD1,
  0x34,0x00,0xDB,0x5A,0xEF,0xAD,0xFB,0x2B,
  0xDF,0x9D,0xFF,0x2B,0xFF,0x2F,0xBF,0xAF,
  0xBF,0x2F,0x85,0xE9,0xFC,0xC6,0xBD,0xFF,
  0xEF,0xFF,0xFF,0xFF,0xBE,0x84,0xFA,0xFF,
  0xC0,0xFF,0x84,0x05,0xFE,0xD0,0xDF,0xFF,
  0xEF,0xFF,0x8F,0xFF,0xCF,0xFF,0xF2,0xFF,
  0xDA,0xFF,0xE8,0xFF,0xEA,0xFF,0xF9,0x86,
  0x3D,0xFF,0xF0,0x08,0xFF,0x81,0xFF,0x64,
  0xFF,0x00,0xFF,0x54,0xFF,0xE2,0xFF,0x7C,
  0xFF,0xBF,0xFF,0x20,0xFF,0x02,0xFF,0x50,
  0xFF,0x04,0xFF,0x41,0xFF,0x17,0xFF,0xAF,
  0xFF,0xBF,0xFF,0x05,0xFF,0x20,0xFF,0x80,
  0xFF,0x12,0xFD,0xF4,0xFF,0x7F,0xFD,0xEF,
  0xFE,0xFB,0xFE,0xFE,0x85,0xD1,0xFB,0x85,
  0x5F,0xFC,0x83,0x6A,0xFB,0xEF,0x80,0x34,
  0xE0,0x75,0x80,0xB4,0x50,0xBB,0x40,0xDD,
  0x20,0xDA,0x20,0xFD,0x10,0xFF,0x00,0x1A,
  0x00,0xFE,0x00,0x35,0x00,0x7E,0x00,0x2A,
  0x00,0xBE,0x00,0x2B,0x00,0xFF,0xAF,0xFF,
  0x1F,0xFF,0xBF,0x7F,0x1F,0xFF,0xFF,0x7F,
  0x3F,0xFF,0x7F,0x7F,0x3F,0xF5,0x86,0xC5,
  0xFE,0xC0,0xEB,0x85,0x41,0xFC,0xC9,0xFF,
  0xCF,0xFF,0xEF,0xFF,0xFB,0xFF,0xFD,0xFF,
  0xF8,0x83,0xFC,0xFF,0xC1,0xFF,0xFC,0x84,
  0x23,0xFC,0xC2,0xFD,0xFF,0x57,0x84,0xC0,
  0xFA,0xC4,0x41,0xFF,0x92,0xFF,0xEB,0x84,
  0x34,0xFF,0xC8,0xAB,0xFF,0x17,0xFF,0x00,
  0xFF,0x42,0xFF,0x20,0x84,0x28,0xFB,0x86,
  0x82,0xFE,0xD4,0xFE,0x1B,0xF0,0x77,0xC0,
  0xFE,0xF5,0xFB,0xEE,0xD6,0xE9,0xFA,0xAD,
  0xF9,0xFE,0xBA,0xED,0xCA,0xB5,0x6A,0x95,
  0x45,0xFF,0x00,0xCC,0xB7,0x00,0xFD,0x00,
  0xDD,0x20,0xEE,0x10,0xFE,0x00,0xFE,0x08,
  0xFE,0x85,0x11,0xFB,0xD0,0x00,0x5D,0x00,
  0xAD,0x00,0xB7,0x00,0xAD,0x00,0xBB,0x00,
  0xAF,0x00,0x3F,0x00,0xCE,0x01,0x43,0xFF,
  0x7F,0x83,0xCF,0xFA,0x8A,0x29,0xFE,0x8C,
  0x20,0xFE,0x83,0x66,0xFB,0x83,0x5A,0xFB,
  0x83,0xC0,0xFA,0x83,0x30,0xFA,0xC1,0x2A,
  0xFF,0x83,0xF0,0xFC,0x85,0x26,0xFA,0x83,
  0xF2,0xFC,0xE5,0x45,0xFE,0x06,0xF8,0x2F,
  0xF0,0x1A,0xE0,0x3E,0xC0,0xEB,0x00,0x77,
  0x80,0xDE,0x01,0xBE,0x01,0xDE,0x01,0x7A,
  0x05,0xBD,0x02,0xF2,0x0D,0x7D,0x02,0x52,
  0xAD,0xAA,0x55,0xA9,0x56,0xAA,0x55,0x95,
  0x6A,0x83,0xF6,0xFF,0xD0,0x55,0xAA,0xBF,
  0x00,0xFD,0x00,0xDF,0x00,0xF7,0x00,0xBD,
  0x00,0xDF,0x00,0xED,0x00,0x77,0x88,0x10,
  0xFA,0x83,0x0E,0xFA,0xD3,0xEF,0x00,0x7F,
  0x00,0x3E,0x01,0xDC,0x03,0xB9,0x07,0xBC,
  0x03,0xF0,0x0F,0xFA,0x07,0xD0,0x2F,0x68,
  0x9F,0x45,0x7F,0xFF,0x83,0x2C,0xFA,0x83,
  0x3C,0xFA,0xFF,0xC0,0xFF,0xE0,0xFF,0xA4,
  0xFF,0xE0,0xFF,0x90,0xFF,0xE3,0xFC,0x11,
  0xFE,0x03,0xFC,0x87,0xF8,0x1E,0xE0,0x3A,
  0xC0,0x7D,0x80,0xEB,0x00,0xDA,0x00,0xDA,
  0x00,0x7B,0x00,0xAA,0x00,0xDF,0x00,0xAB,
  0x00,0x6F,0x00,0x56,0x01,0xDE,0x01,0xE5,
  0x1A,0xFA,0x05,0xCA,0x35,0xF5,0x0A,0x2A,
  0xD5,0xAA,0x55,0xAA,0x55,0xD5,0x2A,0x55,
  0xAA,0x92,0x6D,0x83,0x78,0xFF,0xCC,0xAA,
  0x55,0xAB,0x54,0x52,0xAD,0x54,0xAB,0xBD,
  0x00,0xEF,0x00,0xBB,0x84,0x90,0xF9,0x83,
  0x42,0xFA,0xC4,0x5B,0x00,0xDF,0x00,0x7B,
  0x84,0xC0,0xFA,0xD6,0xFF,0x00,0x7D,0x02,
  0xDE,0x01,0xFC,0x03,0xA0,0x5F,0x82,0x7F,
  0xC0,0x3F,0x89,0x7F,0x20,0xFF,0x04,0xFF,
  0x90,0xFF,0x40,0x8A,0x72,0xFF,0x42,0x3F,
  0xFF,0x88,0x73,0xFE,0xFC,0xFF,0xFF,0xFE,
  0xF5,0xFE,0xFF,0xFA,0xC7,0xF8,0x9F,0xE0,
  0xBD,0xC0,0x76,0x80,0xFA,0x00,0xED,0x00,
  0x5A,0x00,0xED,0x00,0x6A,0x00,0x55,0x00,
  0xAA,0x00,0xD5,0x00,0xAA,0x00,0x55,0x00,
  0xA4,0x00,0x52,0x00,0xAF,0x00,0x5D,0x02,
  0xBE,0x01,0x56,0x09,0x7D,0x02,0x37,0x08,
  0xFA,0x05,0xED,0x12,0x55,0xAA,0x55,0xAA,
  0xD5,0x2A,0x83,0xE6,0xFE,0x83,0x6C,0xFF,
  0xC5,0xB5,0x4A,0x55,0xAA,0x54,0xAB,0x83,
  0x62,0xFF,0xC7,0x55,0xAA,0x48,0xB7,0x2A,
  0xD5,0xA0,0x5F,0x0F,0x00,0x00
};

const palette_color_t* const naija_crystal_tiles_pal[] = {
  /* Gameboy Color palette 0 */
  14659,
  20934,
  29352,
  26351,
  /* Gameboy Color palette 1 */
  0,
  0,
  0,
  0,
};

void init_naija_crystal_tiles(uint8_t tile_start_idx, uint8_t pal_start_idx) BANKED {
  gb_decompress_bkg_data(tile_start_idx, naija_crystal_tiles);
  set_bkg_palette(pal_start_idx, 2, naija_crystal_tiles_pal);
}

/* End of NAIJA_CRYSTAL_TILES.C */
