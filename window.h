#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Window structure.
typedef struct window
{
    SDL_Window *window;
    SDL_GLContext gl_context;
} window_t;

// Window initialization.
int create_window(int width, int height, const char *title, window_t *out);
void destroy_window(window_t *window);

// OpenGL initialization.
// TODO: Separate this from the window code.
int setup_opengl();

// Program main loop.
void enter_main_loop(window_t *window);

#endif // _WINDOW_H_
