#ifndef VVVVVV_CONTROLLER_H
#define VVVVVV_CONTROLLER_H

extern void init_vvvvvv(void) NONBANKED;
extern void update_vvvvvv(uint8_t input) NONBANKED;

extern void fall_down(void) BANKED;
extern void fall_up(void) BANKED;
extern void update_player(uint8_t input) BANKED;
extern void draw_screen(void) NONBANKED;

#endif
