#include "sdl_window.h"
#include "error_stack.h"
#include <stdio.h>
#include <stdlib.h>

// Window constant parameters.
#define WINDOW_BUFFER_DEPTH 24

// Private functions.
static window_event_result_t handle_keyboard_event(const sdl_window_t *window, const SDL_KeyboardEvent *event);
static key_code_t translate_sdl_key(SDL_Keycode sdl_code);

/*
 * Base window initialization for easier cleanup.
 */
void sdl_window_null(sdl_window_t *window)
{
	window_null(&window->base);
	window->sdl_window = NULL;
	window->gl_context = NULL;
}

/*
 * Initialize SDL window with OpenGL context.
 * Returns 1 on success and fills output struct.
 */
int sdl_window_initialize(int width, int height, const char *title, sdl_window_t *out)
{
    SDL_Window *result;
    SDL_GLContext context;

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		error_stack_log("Failed to initialize SDL: %s.\n", SDL_GetError());
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
        error_stack_log("Failed to create OpenGL window.\n");
        return 0;
    }
	out->sdl_window = result;

    // Create OpenGL context.
    context = SDL_GL_CreateContext(result);
    if (context == NULL) {
        error_stack_log("Failed to create OpenGL context: %s.\n", SDL_GetError());
        return 0;
    }
	out->gl_context = context;

	// Enable vertical sync.
	SDL_GL_SetSwapInterval(1);

	// Finished!
    return 1;
}

/*
 * Clean up window and GL context.
 */
void sdl_window_destroy(sdl_window_t *window)
{
    // Destroy context/window.
	if (window->gl_context != NULL) {
		SDL_GL_DeleteContext(window->gl_context);
	}
	if (window->sdl_window != NULL) {
		SDL_DestroyWindow(window->sdl_window);
	}
}

/*
 * Handle events for the window.
 */
window_event_result_t sdl_window_handle_events(sdl_window_t *window)
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		// Trigger close if event is to close window.
		switch (event.type) {
		case SDL_QUIT:
			return WINDOW_EVENT_QUIT;

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
void sdl_window_swap_buffers(sdl_window_t *window)
{
	SDL_GL_SwapWindow(window->sdl_window);
}

/*
 * Handle keyboard event by updating state.
 */
window_event_result_t handle_keyboard_event(const sdl_window_t *window, const SDL_KeyboardEvent *event)
{
	SDL_Keycode sdl_code;
	key_code_t key_code;
	const window_t *base;
	window_event_result_t result;

	// Convert code to engine code.
	sdl_code = event->keysym.sym;
	key_code = translate_sdl_key(sdl_code);
	if (key_code == ENGINE_KEY_INVALID) {
		return WINDOW_EVENT_OK;
	}

	// Send event.
	base = &window->base;
	switch (event->state) {
	case SDL_PRESSED:
		result = window_handle_key_press(base, key_code);
		break;
	case SDL_RELEASED:
		result = window_handle_key_release(base, key_code);
		break;
	default:
		result = WINDOW_EVENT_OK;
		break;
	}
	return result;
}

/*
 * Convert SDL key code to engine key code.
 */
static key_code_t translate_sdl_key(SDL_Keycode sdl_code)
{
	// Alphabet translation.
	if ((sdl_code >= SDLK_a) && (sdl_code <= SDLK_z)) {
		return (key_code_t)(ENGINE_KEY_A + (sdl_code - SDLK_a));
	}

	return (key_code_t)ENGINE_KEY_INVALID;
}
