#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "window_common.h"
#include "opengl_renderer.h"
#include <SDL.h>
#include <SDL_opengl.h>

// Window structure.
typedef struct sdl_window
{
	// Base window parameters.
	window_t base;

	// SDL specific references/parameters.
    SDL_Window *sdl_window;
	SDL_GLContext gl_context;
} sdl_window_t;

// Window initialization.
void sdl_window_null(sdl_window_t *window);
int sdl_window_initialize(int width, int height, const char *title, sdl_window_t *out);
void sdl_window_destroy(sdl_window_t *window);

// Window running functions.
window_event_result_t sdl_window_handle_events(sdl_window_t *window);
void sdl_window_swap_buffers(sdl_window_t *window);

#endif // _WINDOW_H_
