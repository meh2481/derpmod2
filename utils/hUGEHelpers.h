#ifndef HUGE_HELPERS_H
#define HUGE_HELPERS_H
#include "hUGEDriver.h"

// Songs
// extern const hUGESong_t pressure_cooker;
// extern const hUGESong_t space_odyssey;
// extern const hUGESong_t the_traveller;

extern void init_aquaria_music(void) NONBANKED;
BANKREF_EXTERN(aquaria_music)

extern void init_title_music(void) NONBANKED;
BANKREF_EXTERN(title_music)

// extern void init_vvvvvv_music(void) NONBANKED;
// BANKREF_EXTERN(vvvvvv_music)

// extern const hUGESong_t* const space_odyssey_p;

extern void hUGE_stop_music(void) NONBANKED;

#endif
