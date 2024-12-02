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
	#define CBTFX_PLAY_NOTE_C CBTFX_init(&NOTE_C[0])
	extern const unsigned char NOTE_C[];
	#define CBTFX_PLAY_NOTE_D CBTFX_init(&NOTE_D[0])
	extern const unsigned char NOTE_D[];
	#define CBTFX_PLAY_NOTE_E CBTFX_init(&NOTE_E[0])
	extern const unsigned char NOTE_E[];
	#define CBTFX_PLAY_NOTE_F CBTFX_init(&NOTE_F[0])
	extern const unsigned char NOTE_F[];
	#define CBTFX_PLAY_NOTE_G CBTFX_init(&NOTE_G[0])
	extern const unsigned char NOTE_G[];
	#define CBTFX_PLAY_NOTE_A CBTFX_init(&NOTE_A[0])
	extern const unsigned char NOTE_A[];
	#define CBTFX_PLAY_NOTE_B CBTFX_init(&NOTE_B[0])
	extern const unsigned char NOTE_B[];
	#define CBTFX_PLAY_NOTE_HI_C CBTFX_init(&NOTE_HI_C[0])
	extern const unsigned char NOTE_HI_C[];

	extern const unsigned char* NOTE_LIST[];

#endif
