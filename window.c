#include "window.h"
#include <stdio.h>
#include <stdlib.h>

// Window constant parameters.
#define WINDOW_BUFFER_DEPTH 24

// Private functions.
static void handle_keyboard_event(window_t *window, SDL_KeyboardEvent *event);
static key_code_t sdl_key_to_engine(SDL_Keycode sdl_code);

/*
 * Base window initialization for easier cleanup.
 */
void window_null(window_t *window)
{
	window->sdl_window = NULL;
	window->sdl_gl = NULL;
}

/*
 * Initialize SDL window with OpenGL context.
 * Returns 1 on success and fills output struct.
 */
int window_initialize(int width, int height, const char *title, window_t *out)
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Set buffering attributes.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, WINDOW_BUFFER_DEPTH);

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

	// Enable vertical sync.
	SDL_GL_SetSwapInterval(1);

	// Initialize keyboard manager.
	initialize_keyboard_manager(&out->keyboard);

	// Finished!
    return 1;
}

/*
 * Clean up window and GL context.
 */
void window_destroy(window_t *window)
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
int window_handle_events(window_t *window)
{
	(void)window;
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		// Trigger close if event is to close window.
		switch (event.type) {
		case SDL_QUIT:
			return 0;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handle_keyboard_event(window, &event.key);
			break;
		}
	}
	return 1;
}

/*
 * Trigger buffer swap for the window.
 */
void window_swap_buffer(window_t *window)
{
	SDL_GL_SwapWindow(window->sdl_window);
}

/*
 * Handle keyboard event by updating state.
 */
void handle_keyboard_event(window_t *window, SDL_KeyboardEvent *event)
{
	SDL_Keycode sdl_code;
	key_code_t key_code;
	key_state_t old_state;
	key_state_t new_state;

	// Convert code to engine code.
	sdl_code = event->keysym.sym;
	key_code = sdl_key_to_engine(sdl_code);
	if (key_code == ENGINE_KEY_INVALID) {
		return;
	}

	// Update state in keyboard manager.
	old_state = get_key_state(&window->keyboard, key_code);
	if (event->state == SDL_PRESSED) {
		new_state = FLAG_KEY_DOWN;
	}
	else {
		new_state = 0;
	}

	// Update if state changed.
	if ((old_state & new_state) == 0) {
		new_state |= FLAG_KEY_CHANGED;
	}
	update_key_state(&window->keyboard, key_code, new_state);
	
}

/*
 * Convert SDL key code to engine key code.
 */
static key_code_t sdl_key_to_engine(SDL_Keycode sdl_code)
{
	if ((sdl_code >= SDLK_a) && (sdl_code <= SDLK_z)) {
		return (key_code_t)(ENGINE_KEY_A + (sdl_code - SDLK_a));
	}

	return (key_code_t)ENGINE_KEY_INVALID;
}
