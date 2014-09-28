#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "keyboard_manager.h"
#include "opengl_renderer.h"
#include <SDL.h>
#include <SDL_opengl.h>

// Window structure.
typedef struct window
{
    SDL_Window *sdl_window;
	SDL_GLContext sdl_gl;
	keyboard_manager_t keyboard;

	// Window size parameters.
} window_t;

// Window initialization.
void window_null(window_t *window);
int window_initialize(int width, int height, const char *title, window_t *out);
void window_destroy(window_t *window);

// Window running functions.
int window_handle_events(window_t *window);
void window_swap_buffer(window_t *window);

#endif // _WINDOW_H_
