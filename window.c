#include "file.h"
#include "window.h"
#include <stdlib.h>


// Window constant parameters.
#define WINDOW_BUFFER_DEPTH 24

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

    // Create OpenGL context.
    context = SDL_GL_CreateContext(result);
    if (context != NULL) {
        fprintf(stderr, "Failed to create OpenGL context: %s.\n", SDL_GetError());
        SDL_DestroyWindow(result);
        return 0;
    }

    // Fill out struct.
    out->window = result;
    out->gl_context = context;

	// Initialize OpenGL.
	setup_opengl(out);

	// Finished!
    return 1;
}

/*
 * Clean up window and GL context.
 */
void destroy_window(window_t *window)
{
    // Destroy context/window.
    SDL_GL_DeleteContext(window->gl_context);
    SDL_DestroyWindow(window->window);

    // Update pointers.
    window->gl_context = NULL;
    window->window = NULL;
}

/*
 * Set up OpenGL rendering scene.
 * Returns 1 on success, 0 otherwise.
 */
int setup_opengl()
{
	GLchar *vertex_source;
	GLchar *fragment_source;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader;

	// Create vertex shader.
	if (read_file("engine.vert", &vertex_source) == 0) {
		return 0;
	}
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
	return 1;
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

