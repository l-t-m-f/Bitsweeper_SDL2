#include "main.h"

int main (int argc, char **argv)
{
  engine_init ();

  interface_init();

  game_place_mines ();

  game_place_hints ();

  game_hide_tiles ();

  for (uint16_t i = 0; i < TILE_COUNT_W; i++)
    {
      for (uint16_t j = 0; j < TILE_COUNT_H; j++)
        {
          printf ("%d", bitwise_check_bits_at (&bit_board[j][i], 0x50));
          if (bitwise_check_bits_at (&bit_board[j][i], 0x1) == true)
            {
              printf ("B ");
            }
          else
            {
              printf ("  ");
            }
        }
      printf ("\n");
    }

  engine_load_images ();

  printf ("\n bit AND result for bit 0,0 : %d\n", bitwise_check_bits_at (&bit_board[0][0], EXP (0)));

  printf ("%d, %d, %d\n", 0 % 3, 1 % 3, 2 % 3);

  while (Continue == true)
    {
      engine_regenerate_seed ();
      engine_check_for_event ();
      engine_render_loop ();
    }

  return 0;
}