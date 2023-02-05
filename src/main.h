//
// Created by LTMF on 1/27/2023.
//

#ifndef MAIN_H
#define MAIN_H

#include "common.h"

//bitwise externals
extern
bool bitwise_check_bits_at(uint8_t *bits, uint8_t bitmask);

//engine externals
extern SDL_Window *main_window;
extern SDL_Renderer *main_renderer;
extern SDL_Surface *Images[5];

extern MSErrorType
engine_init(void);

extern
void engine_check_for_event(void);
extern
int8_t engine_handle_error (MSErrorType error_type);
extern
void engine_regenerate_seed(void);
extern
void engine_render_loop(void);
extern
void engine_request_exit(void);
extern
void engine_load_images(void);

//game externals
extern uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H];
extern bool began;
extern bool Continue;

extern
void game_place_mines(void);
extern
void game_place_hints (void);
extern
void game_hide_tiles (void);

//interface externals
extern
void interface_init(void);

//util external
extern
uint8_t EXP(int e);

#endif //MAIN_H
