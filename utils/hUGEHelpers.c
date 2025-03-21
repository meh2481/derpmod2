#include "hUGEDriver.h"

void hUGE_stop_music(void) NONBANKED {
  // Not sure why this isn't part of the library; mute all channels
  hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH2, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
}

void hUGE_unmute(void) NONBANKED {
  hUGE_mute_channel(HT_CH1, HT_CH_PLAY);
  hUGE_mute_channel(HT_CH2, HT_CH_PLAY);
  hUGE_mute_channel(HT_CH3, HT_CH_PLAY);
  hUGE_mute_channel(HT_CH4, HT_CH_PLAY);
}

BANKREF_EXTERN(title_music)
BANKREF_EXTERN(aquaria_music)
BANKREF_EXTERN(vvvvvv_music)
BANKREF_EXTERN(pajama_music)
BANKREF_EXTERN(credits_music)

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

extern const hUGESong_t pressure_cooker;

void init_vvvvvv_music(void) NONBANKED {
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(vvvvvv_music));
  hUGE_init(&pressure_cooker);
  SWITCH_ROM(previous_bank);
}

extern const hUGESong_t pajama_jam;

void init_pajama_music(void) NONBANKED {
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(pajama_music));
  hUGE_init(&pajama_jam);
  SWITCH_ROM(previous_bank);
}

extern const hUGESong_t credits_trash;

void init_credits_music(void) NONBANKED {
  // hUGE_unmute();
  uint8_t previous_bank = _current_bank;
  SWITCH_ROM(BANK(credits_music));
  hUGE_init(&credits_trash);
  SWITCH_ROM(previous_bank);
}
