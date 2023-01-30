//
// Created by LTMF on 1/27/2023.
//

#ifndef MAIN_H
#define MAIN_H

#include "include/common.h"

// bitwise externals

extern
bool bitwise_check_bits_at(uint8_t *bits, uint8_t bitmask);

// engine externals
extern SDL_Window *Window;
extern SDL_Renderer *Renderer;
extern SDL_Surface *Images[5];

extern void engine_init(void);
extern void engine_regenerate_seed(void);
extern void engine_render_loop(void);
extern bool engine_check_for_event(void);
extern void engine_request_exit(void);
extern void engine_load_images(void);

// game externals
extern uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H];
extern bool Continue;

extern void game_place_mines(void);
extern void game_place_hints (void);
extern void game_hide_tiles (void);

// interface externals

extern void interface_init(void);

// util external

extern uint8_t EXP(int e);



#endif //MAIN_H
