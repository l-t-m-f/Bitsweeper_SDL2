//
// Created by LTMF on 1/27/2023.
//

#include "../include/game.h"

uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H] = {
    [0 ... TILE_COUNT_W - 1] = {
        [0 ... TILE_COUNT_H - 1] = 0x00
    }
};

bool Began = false;
bool Continue = true;
bool GameOver = false;
bool GameWon = false;

static uint8_t mine_count = 0;

void game_place_mines (void);
void game_place_hints (void);
void game_hide_tiles (void);
tile_secret_type game_get_tile_secret__bit1 (uint8_t *tile);
tile_status_type game_get_tile_reveal__bit2 (uint8_t *tile);
void game_make_board_layout (void);
void game_process_mouse_click (SDL_MouseButtonEvent *mouse_button);

static void add_tile_to_board_layout (int x, int y, SDL_Surface *board_surface_ref);
static void reveal (SDL_Point start_coordinates);
static uint8_t check_adjacent_for_hint (int x, int y);
static void set_tile_hint (uint8_t *tile_ptr, uint8_t neighbor_count);
static SDL_Point get_hint_matching_surface_coordinates (int x, int y);

void game_place_mines (void)
{
  memset (bit_board, 0, TILE_COUNT_W * TILE_COUNT_H * sizeof (uint8_t));
  mine_count = 0;
  while (mine_count < MAX_MINE)
    {
      //engine_regenerate_seed ();
      int rand_x = rand () % TILE_COUNT_W;
      int rand_y = rand () % TILE_COUNT_W;

      if (bitwise_check_bits_at (&bit_board[rand_x][rand_y], 0x01) == false)
        {
          bitwise_plop_bit_at (&bit_board[rand_x][rand_y], 0x01);
          mine_count++;
        }
    }
}

void game_place_hints (void)
{
  for (uint16_t i = 0; i < TILE_COUNT_W; i++)
    {
      for (uint16_t j = 0; j < TILE_COUNT_H; j++)
        {
          if (bitwise_check_bits_at (&bit_board[i][j], EXP (0)) == false)
            {
              uint8_t neighbor_count = check_adjacent_for_hint (i, j);
              if (neighbor_count > 0)
                {
                  set_tile_hint (&bit_board[i][j], neighbor_count);
                }
            }
        }
    }
}

static uint8_t check_adjacent_for_hint (int x, int y)
{
  uint8_t neighbor_count = 0;
  for (int8_t i = -1; i < 2; i++)
    {
      for (int8_t j = -1; j < 2; j++)
        {
          if (x + i < 0 || x + i > TILE_COUNT_W ||
              y + j < 0 || y + j > TILE_COUNT_H)
            {

            }
          else
            {
              if (bitwise_check_bits_at (&bit_board[x + i][y + j], 0x01) == true)
                {
                  neighbor_count++;
                }
            }
        }
    }
  return neighbor_count;
}

static void set_tile_hint (uint8_t *tile_ptr, uint8_t neighbor_count)
{

  switch (neighbor_count)
    {
      case 1:
        {
          bitwise_plop_bit_at (tile_ptr, 0x30);
          break;
        }
      case 2:
        {
          bitwise_plop_bit_at (tile_ptr, 0x50);
          break;
        }
      case 3:
        {
          bitwise_plop_bit_at (tile_ptr, 0x70);
          break;
        }
      case 4:
        {
          bitwise_plop_bit_at (tile_ptr, 0x90);
          break;
        }
      case 5:
        {
          bitwise_plop_bit_at (tile_ptr, 0xB0);
          break;
        }
      case 6:
        {
          bitwise_plop_bit_at (tile_ptr, 0xD0);
          break;
        }
      case 7:
        {
          bitwise_plop_bit_at (tile_ptr, 0xF0);
          break;
        }
      case 8:
        {
          bitwise_plop_bit_at (tile_ptr, 0x10);
          break;
        }
      default:
        {
          SDL_Log ("Error! Default case reached when setting tile hint.\n");
          break;
        }
    }

}

void game_hide_tiles (void)
{
  for (uint16_t i = 0; i < TILE_COUNT_W; i++)
    {
      for (uint16_t j = 0; j < TILE_COUNT_H; j++)
        {
          bitwise_plop_bit_at (&bit_board[i][j], 0x02);
          bitwise_clear_bit_at (&bit_board[i][j], 0x02);
        }
    }
}

void game_make_board_layout (void)
{
  SDL_Surface *board_surface = engine_create_portable_surface (16, 16);
  for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < 16; j++)
        {
          add_tile_to_board_layout (j, i, board_surface);
        }
    }
  App.TextureManager.Textures[BOARD] = SDL_CreateTextureFromSurface (Renderer, board_surface);
  SDL_FreeSurface (board_surface);
}

static void add_tile_to_board_layout (int x, int y, SDL_Surface *board_surface_ref)
{
  int picked_tile_x = 0;

  switch (game_get_tile_secret__bit1 (&bit_board[x][y]))
    {
      case EMPTY:
        {
          SDL_Rect dest_target = {
              .h = 60, .w = 60,
              .x = TILE_PIXEL_SIZE * x, .y = TILE_PIXEL_SIZE * y};
          SDL_Surface *temp = NULL;
          if (game_get_tile_reveal__bit2 (&bit_board[x][y]) == REVEALED)
            {

              // Check if the tile has a hint (it would have bit 0x10 active)
              if (bitwise_check_bits_at (&bit_board[x][y], 0x10) == true)
                {
                  temp = engine_extract_tile (0, get_hint_matching_surface_coordinates (x, y));
                }
              else
                {
                  temp = engine_extract_tile (0, (SDL_Point) {.x = 1, .y = 0});
                }
            }
          else
            {
              temp = engine_extract_tile (0, (SDL_Point) {.x = 0, .y = 0});
            }
          SDL_BlitSurface (temp, NULL, board_surface_ref, &dest_target);
          SDL_FreeSurface (temp);
          break;
        }
      case MINE:
        {
          SDL_Rect dest_target = {
              .h = 60, .w = 60,
              .x = TILE_PIXEL_SIZE * x, .y = TILE_PIXEL_SIZE * y};
          SDL_Surface *temp = NULL;
          if (game_get_tile_reveal__bit2 (&bit_board[x][y]) == REVEALED)
            {
              temp = engine_extract_tile (0, (SDL_Point) {.x = 2, .y = 0});
            }
          else
            {
              temp = engine_extract_tile (0, (SDL_Point) {.x = 0, .y = 0});
            }
          SDL_BlitSurface (temp, NULL, board_surface_ref, &dest_target);
          SDL_FreeSurface (temp);
          break;
        }
      default:
        {
          printf ("Error - Default case reached when placing tile."
                  "Invalid tile data.\n");
          break;
        }
    }

}

static SDL_Point get_hint_matching_surface_coordinates (int x, int y)
{
  if (bitwise_check_bits_at (&bit_board[x][y], 0x20) == true)
    {
      return (SDL_Point) {.x = 5, .y = 0};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0x40) == true)
    {
      return (SDL_Point) {.x = 6, .y = 0};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0x60) == true)
    {
      return (SDL_Point) {.x = 7, .y = 0};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0x80) == true)
    {
      return (SDL_Point) {.x = 8, .y = 0};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0xA0) == true)
    {
      return (SDL_Point) {.x = 5, .y = 1};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0xC0) == true)
    {
      return (SDL_Point) {.x = 6, .y = 1};
    }
  else if (bitwise_check_bits_at (&bit_board[x][y], 0xE0) == true)
    {
      return (SDL_Point) {.x = 7, .y = 1};
    }
  else
    {
      return (SDL_Point) {.x = 8, .y = 1};
    }
}

tile_secret_type game_get_tile_secret__bit1 (uint8_t *tile)
{
  bool DEBUG = false;
  if (DEBUG) printf ("Checking bits of tile %p for %u: ", tile, EXP (0));
  if (bitwise_check_bits_at (tile, EXP (0)) == true)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (tile, EXP (0)));
      return MINE;
    }
  else if (bitwise_check_bits_at (tile, EXP (0)) == false)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (tile, EXP (0)));
      return EMPTY;
    }
}

tile_status_type game_get_tile_reveal__bit2 (uint8_t *tile)
{
  bool DEBUG = false;
  if (DEBUG) printf ("Checking bits of tile %p for %u: ", tile, EXP (0));
  if (bitwise_check_bits_at (tile, EXP (1)) == true)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (tile, EXP (1)));
      return REVEALED;
    }
  else if (bitwise_check_bits_at (tile, EXP (1)) == false)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (tile, EXP (1)));
      return HIDDEN;
    }
}

void game_process_mouse_click (SDL_MouseButtonEvent *mouse_button)
{
  if (mouse_button->button == SDL_BUTTON_LEFT)
    {
      SDL_Log ("Mouse has been clicked (left)!\n");

      int tile_x = App.Mouse.x / TILE_PIXEL_SIZE;
      int tile_y = App.Mouse.y / TILE_PIXEL_SIZE;

      if (!Began)
        {
          do
            {
              game_place_mines ();
              game_place_hints ();
              SDL_Log ("Checking %d, %d\n", tile_x, tile_y - 1);
            }
          while (bitwise_check_bits_at (&bit_board[tile_x][tile_y - 1], HINT_BOOL_BIT)
                 == true
                 || bitwise_check_bits_at (&bit_board[tile_x][tile_y - 1], BOMB_BIT)
                    == true);
        }

      reveal ((SDL_Point) {.x = tile_x, .y = tile_y - 1});

      if (bitwise_check_bits_at (&bit_board[tile_x][tile_y - 1], BOMB_BIT) == true)
        {
          GameOver = true;
        }

      if (tile_y > 0) Began = true;
    }

  else if (mouse_button->button == SDL_BUTTON_RIGHT)
    {
      SDL_Log ("Mouse has been clicked (right)!\n");
      if (Began)
        {
          int tile_x = App.Mouse.x / TILE_PIXEL_SIZE;
          int tile_y = App.Mouse.y / TILE_PIXEL_SIZE;
          bitwise_clear_bit_at (&bit_board[tile_x][tile_y], 0x0002);
        }
    }
}

static void reveal (SDL_Point start_coordinates)
{

  if (start_coordinates.x < 0 || start_coordinates.x > TILE_COUNT_W - 1 ||
      start_coordinates.y < 0 || start_coordinates.y > TILE_COUNT_H - 1)
    {
      return;
    }

  uint8_t *bits = &bit_board[start_coordinates.x][start_coordinates.y];

  if (bitwise_check_bits_at (bits, 0x2) == true)
    {

    }
  else
    {
      if (bitwise_check_bits_at (bits, 0x1) == true)
        {
          bitwise_plop_bit_at (bits, 0x2);
          //gameover
        }
      else
        {
          bitwise_plop_bit_at (bits, 0x2);
          if (bitwise_check_bits_at (bits, 0x10) == false)
            {
              reveal ((SDL_Point) {start_coordinates.x, start_coordinates.y - 1});
              reveal ((SDL_Point) {start_coordinates.x, start_coordinates.y + 1});
              reveal ((SDL_Point) {start_coordinates.x - 1, start_coordinates.y});
              reveal ((SDL_Point) {start_coordinates.x + 1, start_coordinates.y});
            }
        }
    }
}