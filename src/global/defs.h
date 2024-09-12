//
// Created by LTMF on 1/26/2023.
//

#ifndef DEFS_H
#define DEFS_H

#define COLOR_MAX               12
#define FONT_SIZE               48
#define HEADER_PIXEL_HEIGHT     60  // Height of the top Header
#define MAX_BUTTON_COUNT        7   // Maximum number of buttons that can be placed on the stack (in app's buttons)
#define MAX_FONT_COUNT          8
#define MAX_FONT_TEXTURE_COUNT  20
#define MAX_MINE_COUNT          30
#define MAX_TEXTURE_COUNT       20
#define NEIGHBORING_MINE_MAX    8
#define STRING_POOL_WIDTH       8   // How many strings can the string pool accommodate
#define STRING_CHAR_MAX         512 // Maximum number of characters for a single string in the string pool
#define TEXT_COLOR_MAX          7   // Maximum number of colors for text
#define TILE_COUNT_H            16
#define TILE_COUNT_W            16
#define TILE_PIXEL_SIZE         60
#define WINDOW_W                (TILE_COUNT_W * TILE_PIXEL_SIZE)
#define WINDOW_H                ((TILE_COUNT_H * TILE_PIXEL_SIZE) + HEADER_PIXEL_HEIGHT)

typedef enum MSError {
   NONE,
   INIT_FAILED,
   DEFAULT_REACHED,
   MS_ERROR_TYPE_MAX,
} MSErrorType;

typedef struct ErrorWrapper {
   MSErrorType error_type;
   union {
      uint32_t bad_value;
      float bad_value_f;
      double bad_value_lf;
      const char *error_text;
   };
} ErrorWrapper;

typedef enum BitSignificant {
   BOMB_BIT = 0x01, // ...._...1
   VISIBILITY_BIT = 0x02, // ...._..1.

   BOMB_FLAG_BIT = 0x04, // ...._01..
   MAYBE_FLAG_BIT = 0x0C, // ...._11..

   HINT_BOOL_BIT = 0x10, // ...1_....

   HINT_MINE_1 = 0x30, // 0011_....
   HINT_MINE_2 = 0x50, // 0101_....
   HINT_MINE_3 = 0x70, // 0111_....
   HINT_MINE_4 = 0x90, // 1001_....
   HINT_MINE_5 = 0xB0, // 1011_....
   HINT_MINE_6 = 0xD0, // 1101_....
   HINT_MINE_7 = 0xF0, // 1111_....
   HINT_MINE_8 = 0x10, // 0001_....

   BIT_SIGNIFICANT_TYPE_MAX,
} BitSignificantType;

typedef enum ColorName {
   WHITE,
   RED,
   GREEN,
   BLUE,
   YELLOW,
   CYAN,
   MAGENTA,
   ORANGE,
   PURPLE,
   BLACK,
   LIGHT_GRAY,
   DARK_GRAY,
   COLOR_NAME_TYPE_MAX,
} ColorNameType;

typedef enum ImageTextureName {
   BOARD,
   MOUSE_CURSOR,
   TILE_SELECTOR,
   IMAGE_TEXTURE_NAME_TYPE_MAX,
} ImageTextureNameType;

typedef enum TileSecret {
   EMPTY,
   MINE,
   TILE_SECRET_TYPE_MAX,
} TileSecretType;

typedef enum TileStatus {
   REVEALED,
   HIDDEN,
   TILE_STATUS_TYPE_MAX,
} TileStatusType;

typedef enum ButtonState {
   ALONE,
   HOVERED,
   CLICKED,
   BUTTON_STATE_TYPE_MAX,
} ButtonStateType;

typedef struct texture_manager {
   SDL_Texture *textures[MAX_TEXTURE_COUNT];
} TextureManager;

typedef struct font_manager {
   TTF_Font *fonts[MAX_FONT_COUNT];
   SDL_Texture *font_textures[MAX_FONT_TEXTURE_COUNT][TEXT_COLOR_MAX];
} FontManager;

typedef struct mouse {
   int32_t x, y;
} Mouse;

typedef struct button {
   SDL_Rect rectangle;
   ButtonStateType button_state;
   bool is_drawn;
} Button;

typedef struct App {
   Mouse mouse;
   TextureManager texture_manager;
   FontManager font_manager;
   Button buttons[MAX_BUTTON_COUNT];
} App;

typedef struct header {
   SDL_Rect rectangle;
   uint8_t mine_count;
   char message_to_player[80];
} Header;

typedef struct MSErr {
   int8_t kind;
   union {
      uint8_t code;
      const char *message;
   } data;
} MSErr;

#endif //DEFS_H
