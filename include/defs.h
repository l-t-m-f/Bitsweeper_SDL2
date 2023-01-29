//
// Created by LTMF on 1/26/2023.
//

#ifndef DEFS_H
#define DEFS_H

#define TILE_COUNT_H 16
#define TILE_COUNT_W 16
#define TILE_SIZE 60

#define MAX_MINE 30

typedef enum tile_secret {
   EMPTY,
   MINE,
} tile_secret_type;

typedef enum tile_status {
   REVEALED,
   HIDDEN,
} tile_status_type;

typedef struct texture_manager {
   SDL_Texture *GameBoard,
   *Cursor, *TileSelector;
} texture_manager;

typedef struct mouse {
   int32_t x, y;
} mouse;

#endif //DEFS_H
