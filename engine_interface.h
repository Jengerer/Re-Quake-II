#ifndef _ENGINE_INTERFACE_H_
#define _ENGINE_INTERFACE_H_

#include "renderer_interface.h"

// Engine window update flags.
#define ENGINE_WINDOW_FULLSCREEN (1 << 0)
#define ENGINE_WINDOW_BORDERLESS (1 << 1)

// Create/update window size and appearance.
typedef void (*engine_update_window_t)(int width, int height, int flags);

// Engine resource functions.
// TODO: eventually the resource loader should be its own interface.
typedef const renderer_interface_t *(*engine_get_graphics_resources)(void);

// Engine functions/callbacks to be called by game/renderer.
typedef struct engine_interface
{
	// Engine display functions.
	engine_update_window_t update_window;

	// Engine resource functions.
	engine_get_graphics_resources get_graphics_resources;
} engine_interface_t;

// Engine interface initialization.
void engine_interface_null();

// Reference to singleton engine interface.
extern engine_interface_t engine_functions;

#endif // _ENGINE_INTERFACE_H_