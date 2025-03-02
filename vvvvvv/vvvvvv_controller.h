#ifndef VVVVVV_CONTROLLER_H
#define VVVVVV_CONTROLLER_H

extern void init_vvvvvv(void) NONBANKED;
extern void update_vvvvvv(uint8_t input) NONBANKED;

extern void fall_down(void) BANKED;
extern void fall_up(void) BANKED;
extern void update_player(uint8_t input) BANKED;
extern void add_vvvvvv_sprites(uint8_t screenX, uint8_t screenY) BANKED;
extern void check_sprite_collisions(void) BANKED;
extern void draw_screen(void) NONBANKED;

#endif
