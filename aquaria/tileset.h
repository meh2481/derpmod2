/*

 TILESET.H

 Include File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : GB-Compress.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 218

  Palette colors       : Included.
  SGB Palette          : None.
  CGB Palette          : 4 Bits per entry.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/

#ifndef __tileset_h_INCLUDE
#define __tileset_h_INCLUDE
#include <gb/cgb.h>

void init_aquaria_tileset(void) BANKED;
BANKREF_EXTERN(aquaria_tileset)

#endif

/* End of TILESET.H */
