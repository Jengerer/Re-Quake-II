#include "engine.h"
#include "memory_manager.h"
#include <stdio.h>

/*
 * Null engine for safe destruction.
 */
void engine_null(engine_t *engine)
{
	window_null(&engine->window);
	renderer_null_interface(&engine->renderer);
	game_null(&engine->game);
}

/*
 * Initialize engine and game.
 * Engine's configuration struct and renderer interface should be filled out before called.
 */
int engine_initialize(engine_t *engine)
{
	engine_configuration_t *config;
	window_t *window;
	renderer_t *renderer;
	game_t *game;
	const char* title;
	config = &engine->config;
	window = &engine->window;
	renderer = &engine->renderer;
	game = &engine->game;

	// Initialize memory manager.
	memory_manager_initialize();

	// Create SDL window.
	title = game->get_name();
	if (!window_initialize(config->width, config->height, title, window)) {
		return 0;
	}

	// Create renderer.
	if (!renderer->initialize()) {
		// Clean up if partially initialized.
		return 0;
	}

	// Create game.
	if (!game->initialize()) {
		return 0;
	}

	// Load game resources.
	if (!game->load_resources(renderer)) {
		return 0;
	}
	return 1;
}

/*
 * Destroy engine and game.
 */
void engine_destroy(engine_t *engine)
{
	window_t *window;
	renderer_t *renderer;
	game_t *game;

	// Deallocate game resources.
	renderer = &engine->renderer;
	game = &engine->game;
	game->free_resources(renderer);

	// Destroy game.
	game->destroy();

	// Destroy renderer.
	renderer->destroy();

	// Destroy window.
	window = &engine->window;
	window_destroy(window);

	// Dump memory allocation.
	memory_manager_destroy();
}

/*
 * Run engine main loop.
 * Returns 1 if no errors occured, 0 otherwise.
 */
int engine_run(engine_t *engine)
{
	game_t *game;
	window_t *window;
	keyboard_manager_t *keyboard;
	int done;
	
	game = &engine->game;
	window = &engine->window;
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

			// Render a new scene.
			game->render(&engine->renderer);

			// Swap the buffer.
			window_swap_buffer(&engine->window);
		}
	}
	return 1;
}
