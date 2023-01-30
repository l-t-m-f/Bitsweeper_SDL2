//
// Created by LTMF on 1/29/2023.
//

#include "../include/interface.h"

font_manager FontManager;

char ***GameStringPool = NULL;

struct header {
   SDL_Rect rectangle;
   uint8_t mine_count;
   char message_to_player[80];
};

header *Header = NULL;

void interface_init (void);

static void load_fonts (void);

void interface_init (void)
{
  load_fonts();
  Header = malloc (sizeof (header));
  Header->rectangle = (SDL_Rect) {.x = 0, .y = 0, .w = WINDOW_W, .h = HEADER_H};
  Header->mine_count = MAX_MINE;
  char *pointer_to_message = &Header->message_to_player[0];
  pointer_to_message = (char *) "Welcome to minesweeper...";

  // Allocate for the GameStringPool
  GameStringPool = calloc (GAME_STRING_POOL_WIDTH, sizeof (char **));
  for (int i = 0; i < GAME_STRING_POOL_WIDTH; i++)
    {
      GameStringPool[i] = calloc (GAME_STRING_POOL_HEIGHT, sizeof (char *));
      for (int j = 0; j < GAME_STRING_POOL_HEIGHT; j++)
        {
          GameStringPool[i][j] = calloc(GAME_STRING_POOL_MAX_LENGTH, sizeof(char));
        }
    }

  strcpy_s (GameStringPool[0][0], GAME_STRING_POOL_MAX_LENGTH, pointer_to_message);
  printf("%s\n", GameStringPool[0][0]);

  FontManager.FontTextures = calloc(FONT_MAX, sizeof(SDL_Texture *));
  FontManager.FontTextures[0] = engine_make_text_texture (GameStringPool[0][0]);
  if(FontManager.FontTextures[0] == NULL) {
    SDL_Log("Error - Font Texture NULL pointer\n");
  }

}

static void load_fonts (void)
{
  FontManager.Fonts = calloc(FONT_MAX, sizeof(TTF_Font *));
  FontManager.Fonts[0] = TTF_OpenFont ("fonts/honey_bear.ttf", FONT_SIZE);
  if(FontManager.Fonts[0] == NULL) {
      SDL_Log("Error - Font NULL pointer\n");
    }
}