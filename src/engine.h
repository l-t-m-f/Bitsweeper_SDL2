//
// Created by LTMF on 1/27/2023.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "global/common.h"

//game externals
extern
uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H];

extern
bool game_over;

extern
TileSecretType game_get_tile_secret__bit1 (uint8_t *tile);

extern
void game_make_board_layout (void);

extern
void game_process_mouse_click (SDL_MouseButtonEvent *mouse_button);

//interface externals
extern
const char ***GameStringPool;

#endif //ENGINE_H
