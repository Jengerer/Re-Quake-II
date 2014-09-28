#include "engine.h"
#include "engine_interface.h"
#include "game_interface.h"
#include "memory_manager.h"
#include "window.h"
#include <stdio.h>

// Private engine structures.
// Structure for storing engine configuration.
typedef struct engine_configuration
{
	int width;
	int height;
} engine_configuration_t;

// Engine state struct.
typedef struct engine
{
	engine_configuration_t config;
	window_t window;
	renderer_interface_t renderer;
	game_interface_t game;
} engine_t;

// Engine instance.
static engine_t engine;
void engine_null(void);

// Interface functions.
void engine_update_window(int width, int height, int flags);
void engine_initialize_interface(void);

// Null engine for safe destruction.
void engine_null(void)
{
	// Null interface and then fill implemented functions.
	engine_interface_null();
	engine_initialize_interface();
	window_null(&engine.window);
	renderer_null_interface(&engine.renderer);
	game_null(&engine.game);
}

// Start the engine and initialize all of the components.
// This is guaranteed to at least initialize memory manager and engine functions interface.
int engine_initialize(void)
{
	engine_configuration_t *config;
	window_t *window;
	renderer_interface_t *renderer;
	const char* title;
	config = &engine.config;

	// Initialize memory manager.
	memory_manager_initialize();

	// Initialize interface.
	engine_initialize_interface();

	// Create SDL window.
	title = engine.game.get_name();
	if (!window_initialize(config->width, config->height, title, window)) {
		return 0;
	}

	// Create renderer.
	if (!engine.renderer.initialize()) {
		return 0;
	}

	// Create game.
	if (!engine.game.initialize()) {
		return 0;
	}
	return 1;
}

// Destroy engine and game.
void engine_destroy(void)
{
	// Deallocate game resources.
	engine.game.destroy();

	// Destroy renderer.
	engine.renderer.destroy();

	// Destroy window.
	window_destroy(&engine.window);

	// Dump memory allocation/leaks.
	memory_manager_destroy();
}

/*
 * Run engine main loop.
 * Returns 1 if no errors occured, 0 otherwise.
 */
int engine_run(void)
{
	game_interface_t *game;
	window_t *window;
	keyboard_manager_t *keyboard;
	int done;
	
	game = &engine.game;
	window = &engine.window;
	keyboard = &window->keyboard;
	done = 0;
	while (!done) {
		// Handle window events and break if closed.
		if (!window_handle_events(window)) {
			done = 1;
		}
		else {
			// Handle game input.
			game->handle_keyboard(keyboard);

			// Run game pre-frame event.
			if (!game->frame_begin(1.0f / 60.0f)) {
				return 0;
			}

			// Render a new scene.
			game->render();

			// Swap the buffer.
			window_swap_buffer(window);
		}
	}
	return 1;
}

// Initialize interface with implemented functions.
void engine_initialize_interface(void)
{
	engine_functions.update_window = &engine_update_window;
}


// Engine interface function for updating window.
void engine_update_window(int width, int height, int flags)
{
}