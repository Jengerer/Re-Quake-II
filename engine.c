#include "engine.h"
#include <stdio.h>

/*
 * Null engine for safe destruction.
 */
void null_engine(engine_t *engine)
{
	null_window(&engine->window);
	null_renderer(&engine->renderer);
}

/*
 * Initialize engine and game.
 * Engine's configuration struct and renderer interface should be filled out before called.
 */
int initialize_engine(engine_t *engine)
{
	window_t *window;
	engine_configuration_t *config;
	renderer_t *renderer;
	window = &engine->window;
	config = &engine->config;
	renderer = &engine->renderer;

	// Create SDL window.
	if (!create_window(config->width, config->height, config->title, window)) {
		fprintf(stderr, "Failed to initialize window.\n");
		return 0;
	}

	// Create renderer.
	if (!renderer->initialize(&renderer->renderer_context)) {
		// Clean up if partially initialized.
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

	// Destroy renderer.
	renderer = &engine->renderer;
	renderer->destroy(renderer->renderer_context);

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
	int done = 0;
	while (!done) {
		// Handle window events and break if closed.
		if (!handle_window_events(&engine->window)) {
			done = 1;
		}
		else {
			// Swap the buffer.
			swap_buffer(&engine->window);
		}
	}
	return 1;
}
