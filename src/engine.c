//
// Created by LTMF on 1/27/2023.
//

#include "../include/engine.h"

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

SDL_Surface *SurfaceArrayObject[] = {};

texture_manager final_textures = {
    .GameBoard = NULL,
    .Cursor = NULL,
    .TileSelector = NULL
};

SDL_Event event_ptr;

SDL_Color Background = {.r = 25, .g = 155, .b = 0, .a = 255};

mouse Mouse;

void engine_init (void);
void engine_render_loop (void);
void engine_request_exit (void);
void engine_regenerate_seed ();

SDL_Surface *engine_extract_tile (uint32_t atlas_key, SDL_Point tile_coordinates);
void engine_load_images (void);
SDL_Surface *engine_create_portable_surface (uint8_t width, uint8_t height);

static void handle_events (SDL_Event event);
static bool handle_sys_events (SDL_Event event);
static void blit (SDL_Texture *texture, int x, int y, int center);

void engine_init (void)
{
  uint32_t sdl_flags =
      SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO;
  uint32_t window_flags = 0;
  uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;
  int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int mixer_flags = MIX_INIT_OGG;

  if (SDL_Init (sdl_flags) < 0)
    {
      SDL_Log ("Something went wrong initializing SDL! (%s)\n", SDL_GetError ());
    }

  Window = SDL_CreateWindow ("Bitsweeper",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             TILE_SIZE * TILE_COUNT_W,
                             TILE_SIZE * TILE_COUNT_H, window_flags);
  if (Window == NULL)
    {
      SDL_Log ("Something went wrong creating the window! (%s)\n", SDL_GetError ());
      engine_request_exit ();

    }

  Renderer = SDL_CreateRenderer (Window, 0, renderer_flags);

  if (Renderer == NULL)
    {
      SDL_Log ("Something went wrong creating the renderer! (%s)\n", SDL_GetError ());
      engine_request_exit ();
    }

  if (IMG_Init (img_flags) != img_flags)
    {
      SDL_Log ("Something went wrong initializing SDL_image! (%s)\n", IMG_GetError ());
    }

  if (TTF_Init () < 0)
    {
      SDL_Log ("Something went wrong initializing SDL_TTF! (%s)\n", TTF_GetError ());
    }

  if (Mix_Init (mixer_flags) != mixer_flags)
    {
      SDL_Log ("Something went wrong initializing SDL_Mixer! (%s)\n", Mix_GetError ());
    }

  engine_regenerate_seed ();

  SDL_ShowCursor (0);
}

void engine_render_loop (void)
{
  SDL_RenderClear (Renderer);
  SDL_SetRenderDrawColor (Renderer, Background.r, Background.g, Background.b, Background.a);
  game_make_board_layout ();
  blit (final_textures.GameBoard, 0, 0, 0);
  blit (final_textures.TileSelector,
        (Mouse.x / TILE_SIZE) * TILE_SIZE, (Mouse.y / TILE_SIZE) * TILE_SIZE, 0);
  blit (final_textures.Cursor, Mouse.x, Mouse.y, 0);
  SDL_RenderPresent (Renderer);
  SDL_DestroyTexture (final_textures.GameBoard);
}

bool engine_check_for_event (void)
{
  if (SDL_PollEvent (&event_ptr))
    {
      handle_events (event_ptr);
    }
}

void engine_regenerate_seed ()
{
  bool DEBUG = false;
  SDL_GetMouseState (&Mouse.x, &Mouse.y);
  srand ((unsigned int) (Mouse.x ^ Mouse.y));
  if (DEBUG) printf ("New rand seed is: %u \n", (unsigned int) (Mouse.x | Mouse.y));
}

static void handle_events (SDL_Event event)
{

  if (handle_sys_events (event) == 1) return;

  switch (event.type)
    {
      case SDL_MOUSEMOTION:
        {
          //printf("We got a motion event. (%u)\n", event.type);
          //printf("Current mouse position is: (%d, %d)\n", event.motion.x, event.motion.y, (event.motion.x ^ event.motion.y));
          break;
        }
      case SDL_MOUSEBUTTONDOWN:
        {
          game_process_mouse_click (&event.button);
          break;
        }
      default:
        {
          //printf("Unhandled event! (%u)\n", event.type);
          break;
        }
    }
  //printf("Event queue empty.\n");
}

void engine_request_exit (void)
{
  SDL_Quit ();
  exit (0);
}

static bool handle_sys_events (SDL_Event event)
{

  switch (event.type)
    {
      case SDL_AUDIODEVICEADDED:
        {
          //printf("Event: New audio device detected.\n");
          return true;
        }
      case SDL_WINDOWEVENT:
        {
          //printf("Event: Window change detected.\n");
          return true;
        }
      case SDL_TEXTEDITING:
        {
          //printf("Event: Text editing (composition) activated.\n");
          return true;
        }
      case SDL_QUIT:
        {
          //printf("Event: Quit event requested.\n");
          engine_request_exit ();
          return true;
        }
      default:
        return false;
    }
}

void engine_load_images (void)
{
  SurfaceArrayObject[0] = IMG_Load ("gfx/Tiles.png");

  if (SurfaceArrayObject[0] == NULL)
    {
      SDL_Log ("Failed to load image at %s: %s\n", "gfx/Tiles.png", IMG_GetError ());
    }
  else
    {
      SDL_Log ("Loaded successfully: %s\n", "gfx/Tiles.png");
    }

  final_textures.Cursor = SDL_CreateTextureFromSurface (
      Renderer, engine_extract_tile (0, (SDL_Point) {.x = 1, .y = 1}));

  final_textures.TileSelector = SDL_CreateTextureFromSurface (
      Renderer, engine_extract_tile (0, (SDL_Point) {.x = 0, .y = 1}));

//   if ((SDL_SetColorKey(SurfaceArrayObject[0], SDL_TRUE, 0)) < 0)
//      {
//         SDL_Log("Failed to set image '%s''s color key to %d: \n%s\n", "gfx/Tiles.png", 0, SDL_GetError());
//      }

}

SDL_Surface *engine_extract_tile (uint32_t atlas_key, SDL_Point tile_coordinates)
{
  SDL_Rect sourceTarget = {.h = 60, .w = 60,
      .x = (tile_coordinates.x * TILE_SIZE), .y = (tile_coordinates.y * TILE_SIZE)};
  SDL_Surface *tile_surface = engine_create_portable_surface (1, 1);
  SDL_BlitSurface (SurfaceArrayObject[atlas_key], &sourceTarget, tile_surface, NULL);
  return tile_surface;
}

SDL_Surface *engine_create_portable_surface (uint8_t width, uint8_t height)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  return SDL_CreateRGBSurface(
      0, (TILE_SIZE * width), (TILE_SIZE * height),
      32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
  return SDL_CreateRGBSurface (
      0, (TILE_SIZE * width), (TILE_SIZE * height),
      32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif
}

static void blit (SDL_Texture *texture, int x, int y, int center)
{
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  SDL_QueryTexture (texture, NULL, NULL, &dest.w, &dest.h);

  if (center)
    {
      dest.x -= dest.w / 2;
      dest.y -= dest.h / 2;
    }

  SDL_RenderCopy (Renderer, texture, NULL, &dest);
}
