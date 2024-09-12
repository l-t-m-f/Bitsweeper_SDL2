//
// Created by LTMF on 1/27/2023.
//

#include "engine.h"

App app;
SDL_Event last_event;
SDL_Palette *main_palette = NULL;
SDL_Renderer *main_renderer = NULL;
SDL_Window *main_window = NULL;
SDL_Surface *surface_array_object[] = {0};

MSErrorType engine_init(void);
void engine_render_loop(void);
void engine_request_exit(void);
void engine_regenerate_seed(void);
SDL_Surface *engine_extract_tile(uint32_t atlas_key,
                                 SDL_Point tile_coordinates);
void engine_load_images(void);
void engine_check_for_event(void);
SDL_Surface *engine_create_portable_surface(uint8_t width, uint8_t height);
SDL_Texture *engine_make_text_texture(char *text, SDL_Color *color);
int8_t engine_handle_error(MSErrorType error_type);

static void create_main_palette(void);
static void render_buttons(void);
static void handle_events(SDL_Event event);
static bool handle_sys_events(SDL_Event event);
static void blit(SDL_Texture *texture, int x, int y, int center);
static SDL_Texture *surface_to_texture(SDL_Surface *surface,
                                       int destroy_surface);

MSErrorType engine_init(void) {
  MSErrorType result = NONE;
  uint32_t sdl_flags =
      SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO;
  uint32_t window_flags = 0;
  uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;
  int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int mixer_flags = MIX_INIT_OGG;

  if (SDL_Init(sdl_flags) < 0) {
    SDL_Log("Something went wrong initializing SDL! (%s)\n", SDL_GetError());
    result = INIT_FAILED;
  }

  main_window = SDL_CreateWindow("Bitsweeper", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H,
                                 window_flags);
  if (main_window == NULL) {
    SDL_Log("Something went wrong creating the window! (%s)\n", SDL_GetError());
    result = INIT_FAILED;
  } else {
    main_renderer = SDL_CreateRenderer(main_window, 0, renderer_flags);

    if (main_renderer == NULL) {
      SDL_Log("Something went wrong creating the renderer! (%s)\n",
              SDL_GetError());
      result = INIT_FAILED;
    } else {
      if (IMG_Init(img_flags) != img_flags) {
        SDL_Log("Something went wrong initializing SDL_image! (%s)\n",
                IMG_GetError());
        result = INIT_FAILED;
      } else {
        if (TTF_Init() < 0) {
          SDL_Log("Something went wrong initializing SDL_TTF! (%s)\n",
                  TTF_GetError());
          result = INIT_FAILED;
        } else {
          if (Mix_Init(mixer_flags) != mixer_flags) {
            SDL_Log("Something went wrong initializing SDL_Mixer! (%s)\n",
                    Mix_GetError());
            result = INIT_FAILED;
          } else {
            engine_regenerate_seed();
            SDL_ShowCursor(0);
            engine_load_images();
            create_main_palette();
          }
        }
      }
    }
  }
  return result;
}

void engine_render_loop(void) {
  SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
  SDL_RenderClear(main_renderer);
  SDL_SetRenderDrawColor(main_renderer, main_palette->colors[11].r,
                         main_palette->colors[11].g, main_palette->colors[11].b,
                         main_palette->colors[11].a);
  game_make_board_layout();
  blit(app.texture_manager.textures[BOARD], 0, HEADER_PIXEL_HEIGHT, 0);
  blit(app.texture_manager.textures[TILE_SELECTOR],
       (app.mouse.x / TILE_PIXEL_SIZE) * TILE_PIXEL_SIZE,
       (app.mouse.y / TILE_PIXEL_SIZE) * TILE_PIXEL_SIZE, 0);
  blit(app.font_manager.font_textures[0][YELLOW], 0, 0, 0);
  if (game_over) {
    blit(app.font_manager.font_textures[3][RED], WINDOW_W / 2, 450, 1);
    render_buttons();
    blit(app.font_manager.font_textures[4][RED], WINDOW_W / 2, 705, 1);
  }
  // render_buttons ();
  blit(app.texture_manager.textures[MOUSE_CURSOR], app.mouse.x, app.mouse.y, 0);

  SDL_RenderPresent(main_renderer);
  SDL_DestroyTexture(app.texture_manager.textures[0]);
}

void engine_check_for_event(void) {
  if (SDL_PollEvent(&last_event)) {
    handle_events(last_event);
  }
}

void engine_regenerate_seed() {
  bool debug = false;
  SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
  srand((unsigned int)(app.mouse.x ^ app.mouse.y));
#ifdef DEBUG
  if (debug)
    printf("New rand seed is: %u \n",
           (unsigned int)(app.mouse.x | app.mouse.y));
#endif // DEBUG
}

void engine_request_exit(void) {
  SDL_Quit();
  exit(0);
}

int8_t engine_handle_error(MSErrorType error_type) {
  int8_t result = 0;
  switch (error_type) {
  case NONE: {
    break;
  }
  case INIT_FAILED: {
    SDL_Log("Initialization failed!\n");
    break;
  }
  case DEFAULT_REACHED: {
    SDL_Log("Error 'default' switch reached!\n");
    break;
  }
  case MS_ERROR_TYPE_MAX: {
    SDL_Log("Error - invalid 'MS_ERROR_TYPE_MAX' case reached in switch of "
            "function handle_error ().\n");
    result = -1;
    break;
  }
  default: {
    SDL_Log("Error - 'default' case reached in switch of function handle_error "
            "().\n");
    result = -1;
    break;
  }
  }
  return result;
}

void engine_load_images(void) {
  surface_array_object[0] = IMG_Load("data/gfx/Tiles.png");

  if (surface_array_object[0] == NULL) {
    SDL_Log("Failed to load image at %s: %s\n", "data/gfx/Tiles.png",
            IMG_GetError());
  } else {
    SDL_Log("Loaded successfully: %s\n", "data/gfx/Tiles.png");
  }

  app.texture_manager.textures[MOUSE_CURSOR] = SDL_CreateTextureFromSurface(
      main_renderer, engine_extract_tile(0, (SDL_Point){.x = 1, .y = 1}));

  app.texture_manager.textures[TILE_SELECTOR] = SDL_CreateTextureFromSurface(
      main_renderer, engine_extract_tile(0, (SDL_Point){.x = 0, .y = 1}));
}

SDL_Surface *engine_extract_tile(uint32_t atlas_key,
                                 SDL_Point tile_coordinates) {
  SDL_Rect sourceTarget = {.h = 60,
                           .w = 60,
                           .x = (tile_coordinates.x * TILE_PIXEL_SIZE),
                           .y = (tile_coordinates.y * TILE_PIXEL_SIZE)};
  SDL_Surface *tile_surface = engine_create_portable_surface(1, 1);
  SDL_BlitSurface(surface_array_object[atlas_key], &sourceTarget, tile_surface,
                  NULL);
  return tile_surface;
}

SDL_Surface *engine_create_portable_surface(uint8_t width, uint8_t height) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  return SDL_CreateRGBSurface(0, (TILE_PIXEL_SIZE * width),
                              (TILE_PIXEL_SIZE * height), 32, 0xFF000000,
                              0x00FF0000, 0x0000FF00, 0x000000FF);
#else
  return SDL_CreateRGBSurface(0, (TILE_PIXEL_SIZE * width),
                              (TILE_PIXEL_SIZE * height), 32, 0x000000FF,
                              0x0000FF00, 0x00FF0000, 0xFF000000);
#endif
}

SDL_Texture *engine_make_text_texture(char *text, SDL_Color *color) {
  SDL_Surface *surface;
  surface = TTF_RenderUTF8_Blended(app.font_manager.fonts[0], text, *color);
  return surface_to_texture(surface, 1);
}

static void handle_events(SDL_Event event) {

  if (handle_sys_events(event) == 1)
    return;

  switch (event.type) {
  case SDL_MOUSEMOTION: {
    if ((event.motion.x > app.buttons[0].rectangle.x &&
         event.motion.x <
             app.buttons[0].rectangle.x + app.buttons[0].rectangle.w) &&
        (event.motion.y > app.buttons[0].rectangle.y &&
         event.motion.y <
             app.buttons[0].rectangle.y + app.buttons[0].rectangle.h)) {
      app.buttons[0].button_state = HOVERED;
    } else {
      app.buttons[0].button_state = ALONE;
    }
    break;
  }
  case SDL_MOUSEBUTTONDOWN: {
    game_process_mouse_click(&event.button);
    break;
  }
  case SDL_KEYDOWN: {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE: {
      SDL_Quit();
      exit(0);
      break;
    }
    default: {
      break;
    }
    }
  }
  default: {
    // printf("Unhandled event! (%u)\n", event.type);
    break;
  }
  }
  // printf("Event queue empty.\n");
}

static bool handle_sys_events(SDL_Event event) {
  switch (event.type) {
  case SDL_AUDIODEVICEADDED: {
    // printf("Event: New audio device detected.\n");
    return true;
  }
  case SDL_WINDOWEVENT: {
    // printf("Event: main_window change detected.\n");
    return true;
  }
  case SDL_TEXTEDITING: {
    // printf("Event: Text editing (composition) activated.\n");
    return true;
  }
  case SDL_QUIT: {
    // printf("Event: Quit event requested.\n");
    engine_request_exit();
    return true;
  }
  default:
    return false;
  }
}

static void blit(SDL_Texture *texture, int x, int y, int center) {
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

  if (center) {
    dest.x -= dest.w / 2;
    dest.y -= dest.h / 2;
  }

  SDL_RenderCopy(main_renderer, texture, NULL, &dest);
}

static SDL_Texture *surface_to_texture(SDL_Surface *surface,
                                       int destroy_surface) {
  SDL_Texture *texture;
  texture = SDL_CreateTextureFromSurface(main_renderer, surface);

  if (destroy_surface) {
    SDL_FreeSurface(surface);
  }
  return texture;
}

static void render_buttons(void) {
  for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
    if (&app.buttons[i] != NULL && app.buttons[i].is_drawn == true) {
      if(app.buttons[i].button_state == ALONE) {
        SDL_SetRenderDrawColor(main_renderer, 55, 125, 0, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(main_renderer, 200, 125, 0, 255);
      }
      SDL_RenderFillRect(main_renderer, &app.buttons[i].rectangle);
    }
  }
}

/*
 * Create the game's color palette. The palette entries are initialized to white
 * and must be properly set.
 */
static void create_main_palette(void) {
  main_palette = SDL_AllocPalette(COLOR_MAX);
  if (main_palette == NULL) {
    SDL_Log("Error allocating main palette! (%s)\n", SDL_GetError());
  }
  if (SDL_SetPaletteColors(
          main_palette,
          (SDL_Color[]){
              (SDL_Color){.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF}, // WHITE
              (SDL_Color){.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF}, // RED
              (SDL_Color){.r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF}, // GREEN
              (SDL_Color){.r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF}, // BLUE
              (SDL_Color){.r = 0xFF, .g = 0xFF, .b = 0x00, .a = 0xFF}, // YELLOW
              (SDL_Color){.r = 0x00, .g = 0xFF, .b = 0xFF, .a = 0xFF}, // CYAN
              (SDL_Color){
                  .r = 0xFF, .g = 0x00, .b = 0xFF, .a = 0xFF}, // MAGENTA
              (SDL_Color){.r = 0xFF, .g = 0x80, .b = 0x00, .a = 0xFF}, // ORANGE
              (SDL_Color){.r = 0x80, .g = 0x00, .b = 0xFF, .a = 0xFF}, // PURPLE
              (SDL_Color){.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF}, // BLACK
              (SDL_Color){
                  .r = 0x80, .g = 0x80, .b = 0x80, .a = 0x80}, // LIGHT_GRAY
              (SDL_Color){
                  .r = 0x40, .g = 0x40, .b = 0x40, .a = 0xFF}, // DARK_GRAY
          },
          0, COLOR_MAX) < 0) {
    SDL_Log("Error setting palette colors. Not all colors could be set! (%s)",
            SDL_GetError());
  }
}
