//
// Created by LTMF on 1/27/2023.
//

#include "../include/game.h"

uint8_t bit_board[TILE_COUNT_W][TILE_COUNT_H] = {
    [0 ... TILE_COUNT_W - 1] = {
        [0 ... TILE_COUNT_H - 1] = 0x00
    }
};
bool Continue = true;

void game_place_mines (void);
void game_hide_tiles (void);
tile_secret_type game_get_tile_secret__bit1 (uint8_t *tile);
tile_status_type game_get_tile_reveal__bit2 (uint8_t *tile);
void game_make_board_layout (void);
void game_process_mouse_click (SDL_MouseButtonEvent *button);

static void add_tile_to_board_layout (int x, int y, SDL_Surface *board_surface_ref);

void game_place_mines (void)
{
  for (uint16_t i = 0; i < TILE_COUNT_W; i++)
    {
      for (uint16_t j = 0; j < TILE_COUNT_H; j++)
        {
          if (15 < rand () % 20)
            {
              bitwise_plop_bit_at (&bit_board[i][j], 0x0001);
            }
        }
    }
}

void game_hide_tiles (void)
{
  for (uint16_t i = 0; i < TILE_COUNT_W; i++)
    {
      for (uint16_t j = 0; j < TILE_COUNT_H; j++)
        {
          bitwise_plop_bit_at (&bit_board[i][j], 0x0002);
          bitwise_clear_bit_at (&bit_board[i][j], 0x0002);
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
  final_textures.GameBoard = SDL_CreateTextureFromSurface (Renderer, board_surface);
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
              .x = TILE_SIZE * x, .y = TILE_SIZE * y};
          SDL_Surface *temp = NULL;
          if (game_get_tile_reveal__bit2 (&bit_board[x][y]) == REVEALED)
            {
              temp = engine_extract_tile (0, (SDL_Point) {.x = 1, .y = 0});
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
              .x = TILE_SIZE * x, .y = TILE_SIZE * y};
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

tile_secret_type game_get_tile_secret__bit1 (uint8_t *tile)
{
  bool DEBUG = false;
  if (DEBUG) printf ("Checking bits of tile %p for %u: ", tile, EXP (0));
  if (bitwise_check_bits_at (*tile, EXP (0)) == true)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (*tile, EXP (0)));
      return MINE;
    }
  else if (bitwise_check_bits_at (*tile, EXP (0)) == false)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (*tile, EXP (0)));
      return EMPTY;
    }
}

tile_status_type game_get_tile_reveal__bit2 (uint8_t *tile)
{
  bool DEBUG = false;
  if (DEBUG) printf ("Checking bits of tile %p for %u: ", tile, EXP (0));
  if (bitwise_check_bits_at (*tile, EXP (1)) == true)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (*tile, EXP (1)));
      return REVEALED;
    }
  else if (bitwise_check_bits_at (*tile, EXP (1)) == false)
    {
      if (DEBUG) printf ("%d\n", bitwise_check_bits_at (*tile, EXP (1)));
      return HIDDEN;
    }
}

void game_process_mouse_click (SDL_MouseButtonEvent *button)
{
  if (button->button == SDL_BUTTON_LEFT)
    {
      SDL_Log ("Mouse has been clicked (left)!\n");
      int tile_x = Mouse.x / TILE_SIZE;
      int tile_y = Mouse.y / TILE_SIZE;
      bitwise_plop_bit_at (&bit_board[tile_x][tile_y], 0x0002);
    }
  else if (button->button == SDL_BUTTON_RIGHT)
    {
      SDL_Log ("Mouse has been clicked (right)!\n");
      int tile_x = Mouse.x / TILE_SIZE;
      int tile_y = Mouse.y / TILE_SIZE;
      bitwise_clear_bit_at (&bit_board[tile_x][tile_y], 0x0002);
    }
}