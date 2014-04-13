#include "window.h"
#include <stdlib.h>

// Window constant parameters.
#define WINDOW_BUFFER_DEPTH 24

/*
 * Base window initialization for easier cleanup.
 */
void null_window(window_t *window)
{
	window->window = NULL;
	window->gl_context = NULL;
	null_opengl_state(&window->opengl_state);
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
	out->window = result;

    // Create OpenGL context.
    context = SDL_GL_CreateContext(result);
    if (context != NULL) {
        fprintf(stderr, "Failed to create OpenGL context: %s.\n", SDL_GetError());
        return 0;
    }
	out->gl_context = context;

	// Initialize OpenGL.
	if (!initialize_opengl(&out->opengl_state)) {
		fprintf(stderr, "Failed to initialize OpenGL.\n");
		return 0;
	}

	// Finished!
    return 1;
}

/*
 * Clean up window and GL context.
 */
void destroy_window(window_t *window)
{
    // Destroy context/window.
	if (window->gl_context != NULL) {
		SDL_GL_DeleteContext(window->gl_context);
	}
	if (window->window != NULL) {
		SDL_DestroyWindow(window->window);
	}
	null_window(window);
}

/*
 * Run window main loop.
 */
void enter_main_loop(window_t *window)
{
    SDL_Event event;

    // Run until quit.
    while (1) {
        // Handle any window events.
        if (SDL_PollEvent(&event)) {
            // End loop if exiting.
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        // Swap buffers.
        SDL_GL_SwapWindow(window->window);
    }
}

