/*

	SFX_0A

	Sound Effect File.

	Info:
		Length			:	4
		Priority		:	0
		Channels used	:	Noise channel
		SGB Support		:	No
*/

#ifndef __SFX_INCLUDE
#define __SFX_INCLUDE

	#include "cbtfx.h"

	#define CBTFX_PLAY_SFX_START_BUTTON CBTFX_init(&start_button[0])
	extern const unsigned char start_button[];

#endif
