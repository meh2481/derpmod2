#include "hUGEDriver.h"

void hUGE_stop_music(void) NONBANKED {
  // Not sure why this isn't part of the library; mute all channels
  hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH2, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
}

BANKREF_EXTERN(title_music)
BANKREF_EXTERN(aquaria_music)

extern const hUGESong_t space_odyssey;

void init_title_music(void) NONBANKED {
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(title_music));
  hUGE_init(&space_odyssey);
  SWITCH_ROM(previous_bank);
}

extern const hUGESong_t the_traveller;

void init_aquaria_music(void) NONBANKED {
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(aquaria_music));
  hUGE_init(&the_traveller);
  SWITCH_ROM(previous_bank);
}
