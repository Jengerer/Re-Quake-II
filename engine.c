#include "engine.h"
#include <stdio.h>

/*
 * Null engine for safe destruction.
 */
void null_engine(engine_t *engine)
{
	null_window(&engine->window);
	null_renderer(&engine->renderer);
	null_game(&engine->game);
}

/*
 * Initialize engine and game.
 * Engine's configuration struct and renderer interface should be filled out before called.
 */
int initialize_engine(engine_t *engine)
{
	engine_configuration_t *config;
	window_t *window;
	renderer_t *renderer;
	game_t *game;
	config = &engine->config;
	window = &engine->window;
	renderer = &engine->renderer;
	game = &engine->game;

	// Create SDL window.
	if (!create_window(config->width, config->height, config->title, window)) {
		return 0;
	}

	// Create renderer.
	if (!renderer->initialize()) {
		// Clean up if partially initialized.
		return 0;
	}

	// Create game.
	if (!game->initialize(&game->context)) {
		return 0;
	}

	// Load game resources.
	if (!game->load_resources(game->context, renderer)) {
		return 0;
	}
	return 1;
}

/*
 * Destroy engine and game.
 */
void destroy_engine(engine_t *engine)
{
	window_t *window;
	renderer_t *renderer;
	game_t *game;

	// Deallocate game resources.
	renderer = &engine->renderer;
	game = &engine->game;
	game->free_resources(game->context, renderer);

	// Destroy game.
	game->destroy(game->context);

	// Destroy renderer.
	renderer->destroy();

	// Destroy window.
	window = &engine->window;
	destroy_window(window);
}

/*
 * Run engine main loop.
 * Returns 1 if no errors occured, 0 otherwise.
 */
int run_engine(engine_t *engine)
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
		if (!handle_window_events(window)) {
			done = 1;
		}
		else {
			// Handle game input.
			game->handle_keyboard(game->context, keyboard);

			// Call game's render function.
			game->render(game->context, &engine->renderer);

			// Swap the buffer.
			swap_buffer(&engine->window);
		}
	}
	return 1;
}
