//
// Created by LTMF on 1/27/2023.
//

#ifndef GAME_H
#define GAME_H

#include "common.h"

uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H];

// bitwise externals
extern bool bitwise_check_bits_at(uint8_t bits, uint8_t bitmask);
extern void bitwise_plop_bit_at(uint8_t *bits, uint8_t bitmask);
extern void bitwise_clear_bit_at(uint8_t *bits, uint8_t bitmask);

// engine externals
extern SDL_Renderer *Renderer;
extern texture_manager final_textures;
extern mouse Mouse;

extern SDL_Surface *engine_create_portable_surface (uint8_t width, uint8_t height);
extern SDL_Surface *engine_extract_tile (uint32_t atlas_key, SDL_Point tile_coordinates);

// util externals
extern uint8_t EXP(int e);

#endif //GAME_H
