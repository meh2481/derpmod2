#include "hUGEDriver.h"

void hUGE_stop_music(void) NONBANKED {
  // Not sure why this isn't part of the library; mute all channels
  hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH2, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
  hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
}
