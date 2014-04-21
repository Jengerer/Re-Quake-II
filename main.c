#include "engine.h"
#include "opengl_renderer.h"
#include "platformer.h"
#include <stdio.h>

// Application constants.
#define APPLICATION_TITLE "Engine Test"
#define APPLICATION_WIDTH 1024
#define APPLICATION_HEIGHT 768

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// Engine state.
	engine_t engine;
	null_engine(&engine);

	// Create configuration for engine.
	engine_configuration_t *config = &engine.config;
	config->title = APPLICATION_TITLE;
	config->width = APPLICATION_WIDTH;
	config->height = APPLICATION_HEIGHT;

	// Set OpenGL as renderer.
	initialize_opengl_interface(&engine.renderer);

	// Load platformer as game.
	initialize_platformer_interface(&engine.game);

	// Initialize engine.
	if (!initialize_engine(&engine)) {
		// Destroy partially initialized state.
		destroy_engine(&engine);
		return -1;
	}

	// Run engine and shut down.
	run_engine(&engine);
	destroy_engine(&engine);

	return 0;
}

