//
// Created by LTMF on 1/26/2023.
//

#ifndef DEFS_H
#define DEFS_H

#define TILE_COUNT_H 16
#define TILE_COUNT_W 16
#define TILE_SIZE 60
#define HEADER_H 60

#define GAME_STRING_POOL_WIDTH 8
#define GAME_STRING_POOL_HEIGHT 1
#define GAME_STRING_POOL_MAX_LENGTH 40

#define FONT_SIZE 48

#define FONT_MAX 8

#define WINDOW_W (TILE_COUNT_W * TILE_SIZE)
#define WINDOW_H (TILE_COUNT_H * TILE_SIZE) + HEADER_H

#define MAX_MINE 30

#define WHITE (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255 }
#define RED (SDL_Color){.r = 255, .g = 0, .b = 0, .a = 255 }
#define GREEN (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255 }
#define BLUE (SDL_Color){.r = 0, .g = 0, .b = 255, .a = 255 }
#define YELLOW (SDL_Color){.r = 255, .g = 255, .b = 0, .a = 255 }
#define CYAN (SDL_Color){.r = 0, .g = 255, .b = 255, .a = 255 }
#define MAGENTA (SDL_Color){.r = 255, .g = 0, .b = 255, .a = 255 }

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

typedef struct font_manager {
   TTF_Font **Fonts; // Like a 1-D Array
   SDL_Texture **FontTextures; //Like a 2-D Array
} font_manager;

typedef struct mouse {
   int32_t x, y;
} mouse;

typedef struct header header;

#endif //DEFS_H
