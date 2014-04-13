#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "opengl_renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Window structure.
typedef struct window
{
    SDL_Window *window;
    SDL_GLContext gl_context;
	opengl_state_t opengl_state;
} window_t;

// Window initialization.
void null_window(window_t *window);
int create_window(int width, int height, const char *title, window_t *out);
void destroy_window(window_t *window);

// Program main loop.
void enter_main_loop(window_t *window);

#endif // _WINDOW_H_
