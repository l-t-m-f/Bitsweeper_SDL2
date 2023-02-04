//
// Created by LTMF on 1/29/2023.
//

#include "../include/interface.h"

char ***GameStringPool = NULL;

header *Header = NULL;

SDL_Color ColorPalette[COLOR_MAX] = {
    (SDL_Color) {.r = 255, .g = 255, .b = 255, .a = 255},
    (SDL_Color) {.r = 255, .g = 0, .b = 0, .a = 255},
    (SDL_Color) {.r = 0, .g = 255, .b = 0, .a = 255},
    (SDL_Color) {.r = 0, .g = 0, .b = 255, .a = 255},
    (SDL_Color) {.r = 255, .g = 255, .b = 0, .a = 255},
    (SDL_Color) {.r = 0, .g = 255, .b = 255, .a = 255},
    (SDL_Color) {.r = 255, .g = 0, .b = 255, .a = 255},
};

void interface_init (void);

static void load_fonts (void);
static void allocate_and_load_string_pools (void);
static char *load_data (void);

void interface_init (void)
{
  load_fonts ();
  Header = malloc (sizeof (header));
  Header->rectangle = (SDL_Rect) {.x = 0, .y = 0, .w = WINDOW_W, .h = HEADER_PIXEL_HEIGHT};
  Header->mine_count = MAX_MINE;

  allocate_and_load_string_pools ();

  App.Buttons[0] = (button) {
      .rectangle = (SDL_Rect) {
          .x = WINDOW_W / 2, .y = 400,
          . w = 200, .h = 50,},
      .button_state = ALONE,
      .is_drawn = true
  };
}

static void load_fonts (void)
{
  App.FontManager.Fonts[0] = TTF_OpenFont ("fonts/Daydream.ttf", FONT_SIZE);
  if (App.FontManager.Fonts[0] == NULL)
    {
      SDL_Log ("Error - Font NULL pointer\n");
    }
}

static void allocate_and_load_string_pools (void)
{

  GameStringPool = calloc (GAME_STRING_POOL_SIZE, sizeof (char **));
  for (int i = 0; i < COLOR_MAX; i++)
    {
      GameStringPool[i] = calloc (COLOR_MAX, sizeof (char *));
    }
  char *string_data = load_data ();
  size_t total = strlen (string_data);

  size_t start = 0;
  size_t end = 0;
  uint8_t word = 0;

  char *c = string_data;

  while (c <= string_data + total)
    {
      if (*c == '\n' || c == string_data + total)
        {
          for (int i = 0; i < COLOR_MAX; i++)
            {
              // Allocate a new element in the string pool of size end plus the nul terminator
              GameStringPool[word][i] = calloc (end + 1, sizeof (char));
              strncpy (GameStringPool[word][i], string_data + start, end);
              GameStringPool[word][i][end] = '\0'; // adding the terminator

              //printf ("The next word %s is %lld letter(s)\n", GameStringPool[word][i], end);

              App.FontManager.FontTextures[word][i] = engine_make_text_texture (
                  GameStringPool[word][i], &ColorPalette[i]);
              if (App.FontManager.FontTextures[word][i] == NULL)
                {
                  SDL_Log ("Error - Font Texture NULL pointer\n");
                }
            }
          start += end;

          // Skip the \n
          start += 2;

          end = 0;
          word++;
        }
      else if (*c != '\r')
        {
          end++;
        }
      c++;
    }
}

static char *load_data (void)
{
  char *buffer = NULL;
  uint64_t length;
  FILE *f = fopen ("dat/strings.txt", "rb");

  if (f != NULL)
    {
      fseek (f, 0, SEEK_END);
      length = (uint64_t) ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length);

      if (buffer != NULL)
        {
          fread (buffer, 1, length, f);
        }
      fclose (f);
    }
  if (buffer != NULL)
    {
      return buffer;
    }
}