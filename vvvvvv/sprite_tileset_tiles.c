/*

 SPRITE_TILESET_TILES.C

 Tile Source File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : GB-Compress.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 23

  Palette colors       : Included.
  SGB Palette          : None.
  CGB Palette          : Constant per entry.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/
#pragma bank 255
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/gbdecompress.h>

/* Start of tile array. */
const unsigned char sprite_tileset_tiles[] =
{
  0xCF,0x00,0x7E,0x00,0xFF,0x36,0xC9,0x36,
  0xDD,0x00,0xFF,0x00,0xC3,0x00,0x66,0x00,
  0x3C,0x83,0xF0,0xFF,0xCB,0x00,0xFF,0x00,
  0xBD,0x00,0x3C,0x00,0x24,0x00,0x66,0x00,
  0x66,0x02,0x00,0x97,0xDE,0xFF,0xC4,0x66,
  0x00,0xC3,0x00,0xC3,0x83,0xC0,0xFF,0xC9,
  0x7E,0xFF,0xB6,0xFF,0x00,0xFF,0x3C,0xFF,
  0x18,0x7E,0x97,0xC0,0xFF,0x94,0xDE,0xFF,
  0x85,0xC0,0xFF,0x8F,0xE0,0xFF,0xC8,0x3D,
  0x00,0x7F,0x00,0xFF,0x00,0xFE,0x00,0xBC,
  0x96,0xE0,0xFF,0xC0,0xBD,0x84,0x60,0xFF,
  0x83,0x96,0xFF,0x88,0x80,0xFF,0xC9,0x00,
  0xEB,0x00,0xC9,0x00,0xFF,0x00,0xE7,0x00,
  0x42,0x94,0x40,0xFF,0x84,0x3D,0xFF,0xC0,
  0xB6,0x1C,0x00,0xC0,0x3C,0x46,0x00,0x18,
  0xC3,0x00,0x1F,0x00,0x1F,0x9A,0xE0,0xFF,
  0xC6,0x98,0x00,0x98,0x00,0xBC,0x00,0x80,
  0x86,0xAE,0xFF,0x44,0x42,0x00,0xC0,0x7A,
  0x94,0x9C,0xFF,0xCA,0x78,0x00,0xFD,0x00,
  0xBF,0x00,0xFF,0x00,0xFD,0x00,0x78,0x94,
  0x7C,0xFF,0xC2,0x71,0x00,0xFB,0x84,0xE0,
  0xFF,0xC2,0xFB,0x00,0x71,0x94,0x66,0xFF,
  0xC4,0x46,0x38,0xDB,0x24,0xC7,0x83,0xFC,
  0xFF,0xC0,0x38,0x83,0xA1,0xFE,0x91,0x46,
  0xFF,0xC5,0x7E,0x00,0xFF,0x1C,0xFF,0x12,
  0x83,0xFC,0xFF,0xC1,0x7E,0x1C,0x91,0xE0,
  0xFF,0x00
};

const palette_color_t* const vvvvvvSpritePal[] = {
  0,
  32767,
  25591,
  26441,
  0,
  95,
  32767,
  26425,
  0,
  0,
  24319,
  0
};

void init_vvvvvv_sprite_tiles(void) BANKED {
  gb_decompress_sprite_data(0, sprite_tileset_tiles);
}

void init_vvvvvv_sprite_palettes(uint8_t pal) BANKED {
  set_sprite_palette(pal, 3, vvvvvvSpritePal);
}

/* End of SPRITE_TILESET_TILES.C */
