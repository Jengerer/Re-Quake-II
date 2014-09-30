#include "engine.h"
#include "memory_manager.h"
#include "sdl_window.h"
#include <stdio.h>

// Engine instance.
engine_t engine;

// Private functions.
void engine_null(void);

// Null engine for safe destruction.
void engine_null(void)
{
	// Null interface and then fill implemented functions.
	sdl_window_null(&engine.window);
}

// Start the engine and initialize all of the components.
// This is guaranteed to be called, and will at least null and initialize memory manager.
int engine_initialize(void)
{
	// Null the engine and propagate it.
	engine_null();

	// Initialize memory manager.
	memory_manager_initialize();

	// We're done for now, pass initialization to listener.
	if (!engine.listener.on_initialize()) {
		return 0;
	}
	return 1;
}

// Destroy engine.
void engine_shutdown(void)
{
	// Tell listener to shut down first.
	engine.listener.on_shutdown();

	// Destroy renderer and window.
	engine.renderer.destroy();
	sdl_window_destroy(&engine.window);

	// Dump memory allocation/leaks.
	memory_manager_destroy();
}

/*
 * Run engine main loop.
 * Returns 1 if no errors occured, 0 otherwise.
 */
int engine_run(void)
{
	engine_listener_t *listener;
	sdl_window_t *window;
	window_event_result_t result;
	int done;
	
	listener = &engine.listener;
	window = &engine.window;
	done = 0;
	while (!done) {
		// Handle window events and break if closed.
		result = sdl_window_handle_events(window);
		switch (result) {
		case WINDOW_EVENT_OK:
			break;
		case WINDOW_EVENT_ERROR:
			return 0;
		case WINDOW_EVENT_QUIT:
			return 1;
		default:
			break;
		}
		
		// Run game pre-frame event.
		if (!listener->on_tick(1.0f / 60.0f)) {
			return 0;
		}
	}
	return 1;
}

// Engine create window.
int engine_create_window(
	const char *title,
	int width,
	int height,
	int flags)
{
	(void)title;
	(void)width;
	(void)height;
	(void)flags;
	return 1;
}

// Engine interface function for updating window.
int engine_update_window(int width, int height, int flags)
{
	(void)width;
	(void)height;
	(void)flags;
	return 1;
}

// Swap buffers in window.
void engine_swap_buffers(void)
{
	sdl_window_swap_buffers(&engine.window);
}

// Get reference to renderer interface.
const renderer_t *engine_get_renderer(void)
{
	return &engine.renderer;
}

// Get reference to renderer shader utilities
const renderer_shader_utilities_t *engine_get_shader_utilities(void)
{
	return &engine.shaders;
}
