//
// Created by LTMF on 1/27/2023.
//

#ifndef GAME_H
#define GAME_H

#include "global/common.h"

uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H];

// bitwise externals
extern bool bitwise_check_bits_at(uint8_t *bits, uint8_t bitmask);
extern void bitwise_plop_bit_at(uint8_t *bits, uint8_t bitmask);
extern void bitwise_clear_bit_at(uint8_t *bits, uint8_t bitmask);

// engine externals
extern App app;
extern SDL_Renderer *main_renderer;

extern MSErrorType engine_init(void);
extern int8_t engine_handle_error(MSErrorType error_type);
extern void engine_request_exit(void);

extern SDL_Surface *engine_create_portable_surface(uint8_t width,
                                                   uint8_t height);
extern SDL_Surface *engine_extract_tile(uint32_t atlas_key,
                                        SDL_Point tile_coordinates);
extern int8_t engine_handle_error(MSErrorType error_type);
extern void engine_regenerate_seed();

// util externals
extern uint8_t EXP(int e);

extern void interface_init(void);

#endif // GAME_H
