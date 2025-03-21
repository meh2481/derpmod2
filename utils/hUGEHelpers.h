#ifndef HUGE_HELPERS_H
#define HUGE_HELPERS_H
#include "hUGEDriver.h"

// Songs
extern void init_aquaria_music(void) NONBANKED;
BANKREF_EXTERN(aquaria_music)

extern void init_title_music(void) NONBANKED;
BANKREF_EXTERN(title_music)

extern void init_vvvvvv_music(void) NONBANKED;
BANKREF_EXTERN(vvvvvv_music)

extern void init_pajama_music(void) NONBANKED;
BANKREF_EXTERN(pajama_music)

extern void init_credits_music(void) NONBANKED;
BANKREF_EXTERN(credits_music)

extern void hUGE_stop_music(void) NONBANKED;

#endif
