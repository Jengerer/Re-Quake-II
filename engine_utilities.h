#ifndef _ENGINE_UTILITIES_H_
#define _ENGINE_UTILITIES_H_

#include "renderer.h"

// Create the window.
typedef int (*engine_create_window_t)(
	const char *title,
	int width,
	int height,
	int flags);

// Create/update window size and appearance.
typedef int (*engine_update_window_t)(
	int width,
	int height,
	int flags);

// Get the engine renderer interface.
typedef const renderer_t *(*engine_get_renderer_t)(void);

// Engine functions and utilities to be called by game/renderer.
typedef struct engine_utilities
{
	// Engine display functions.
	engine_create_window_t create_window;
	engine_update_window_t update_window;

	// Rendering related functions.
	engine_get_renderer_t get_renderer;
} engine_utilities_t;

#endif // _ENGINE_UTILITIES_H_