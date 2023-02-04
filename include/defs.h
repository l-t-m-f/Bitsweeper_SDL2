//
// Created by LTMF on 1/26/2023.
//

#ifndef DEFS_H
#define DEFS_H

#define FONT_SIZE 48

#define GAME_STRING_POOL_SIZE 8
#define COLOR_MAX 7

#define BUTTON_MAX 7

#define HEADER_PIXEL_HEIGHT 60

#define TILE_COUNT_H 16
#define TILE_COUNT_W 16
#define TILE_PIXEL_SIZE 60

#define MAX_MINE 30

#define MAX_FONT_COUNT 8
#define MAX_FONT_TEXTURE_COUNT 20
#define MAX_TEXTURE_COUNT 20

#define WINDOW_W (TILE_COUNT_W * TILE_PIXEL_SIZE)
#define WINDOW_H (TILE_COUNT_H * TILE_PIXEL_SIZE) + HEADER_PIXEL_HEIGHT

enum bit_significance {
   BOMB_BIT = 0x01,
   VISIBILITY_BIT = 0x02,
   FLAG_BIT_1 = 0x04,
   FLAG_BIT_2 = 0x08,
   HINT_BOOL_BIT = 0x10,
   HINT_BIT_1 = 0x20,
   HINT_BIT_2 = 0x40,
   HINT_BIT_3 = 0x80
};

enum color_name {
   RGBA_WHITE,
   RGBA_RED,
   RGBA_GREEN,
   RGBA_BLUE,
   RGBA_YELLOW,
   RGBA_CYAN,
   RGBA_MAGENTA
};

enum image_texture_name_type {
   BOARD,
   MOUSE_CURSOR,
   TILE_SELECTOR,
   MAX_IMAGE_TEXTURE_NAME_TYPE,
};

typedef enum tile_secret {
   EMPTY,
   MINE,
   MAX_TILE_SECRET_TYPE,
} tile_secret_type;

typedef enum tile_status {
   REVEALED,
   HIDDEN,
   MAX_TILE_STATUS_TYPE,
} tile_status_type;

typedef enum button_state {
   ALONE,
   HOVERED,
   CLICKED,
   MAX_BUTTON_STATE_TYPE,
} button_state_type;

typedef struct texture_manager {
   SDL_Texture *Textures[MAX_TEXTURE_COUNT];
} texture_manager;

typedef struct font_manager {
   TTF_Font *Fonts[MAX_FONT_COUNT];
   SDL_Texture *FontTextures[MAX_FONT_TEXTURE_COUNT][COLOR_MAX];
} font_manager;

typedef struct mouse {
   int32_t x, y;
} mouse;

typedef struct button {
   SDL_Rect rectangle;
   button_state_type button_state;
   bool is_drawn;
} button;

typedef struct app {
   mouse Mouse;
   texture_manager TextureManager;
   font_manager FontManager;
   button Buttons[BUTTON_MAX];
} app;

typedef struct header {
   SDL_Rect rectangle;
   uint8_t mine_count;
   char message_to_player[80];
} header;

#endif //DEFS_H
