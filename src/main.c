#include "main.h"

int
main (int argc, char **argv)
{
  if(engine_handle_error(engine_init ()) < 0) engine_request_exit();
  interface_init ();
  game_hide_tiles ();

  while (Continue == true)
    {
      engine_regenerate_seed ();
      engine_check_for_event ();
      engine_render_loop ();
    }
    for(int i = 0; i < argc; i++)
    {
      SDL_Log("%s\n", argv[i]);
    }
  return 0;
}