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
} window_t;

// Window initialization.
void null_window(window_t *window);
int create_window(int width, int height, const char *title, window_t *out);
void destroy_window(window_t *window);

// Window running functions.
int handle_window_events(window_t *window);
void swap_buffer(window_t *window);

#endif // _WINDOW_H_
