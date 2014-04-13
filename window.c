#include "window.h"
#include <stdio.h>
#include <stdlib.h>

// Window constant parameters.
#define WINDOW_BUFFER_DEPTH 24

/*
 * Base window initialization for easier cleanup.
 */
void null_window(window_t *window)
{
	window->sdl_window = NULL;
	window->sdl_gl = NULL;
}

/*
 * Initialize SDL window with OpenGL context.
 * Returns 1 on success and fills output struct.
 */
int create_window(int width, int height, const char *title, window_t *out)
{
    SDL_Window *result;
    SDL_GLContext context;

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s.\n", SDL_GetError());
        return 0;
    }

	// Set GL version.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Set buffering attributes.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, WINDOW_BUFFER_DEPTH);

	// Enable vertical sync.
	SDL_GL_SetSwapInterval(1);

    // Create SDL window.
    result = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (result == NULL) {
        fprintf(stderr, "Failed to create OpenGL window.\n");
        return 0;
    }
	out->sdl_window = result;

    // Create OpenGL context.
    context = SDL_GL_CreateContext(result);
    if (context == NULL) {
        fprintf(stderr, "Failed to create OpenGL context: %s.\n", SDL_GetError());
        return 0;
    }
	out->sdl_gl = context;

	// Finished!
    return 1;
}

/*
 * Clean up window and GL context.
 */
void destroy_window(window_t *window)
{
    // Destroy context/window.
	if (window->sdl_gl != NULL) {
		SDL_GL_DeleteContext(window->sdl_gl);
	}
	if (window->sdl_window != NULL) {
		SDL_DestroyWindow(window->sdl_window);
	}
}

/*
 * Handle events for the window.
 * Returns 0 only if window close event was triggered, 1 otherwise.
 */
int handle_window_events(window_t *window)
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		// Trigger close if event is to close window.
		if (event.type == SDL_QUIT) {
			return 0;
		}
	}
	return 1;
}

/*
 * Trigger buffer swap for the window.
 */
void swap_buffer(window_t *window)
{
	SDL_GL_SwapWindow(window->sdl_window);
}
