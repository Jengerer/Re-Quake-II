#include "engine.h"
#include "opengl_renderer.h"
#include "arpg_game.h"
#include <stdio.h>

// Application constants.
#define APPLICATION_TITLE "Engine Test"
#define APPLICATION_WIDTH 1024
#define APPLICATION_HEIGHT 768

int main(int argc, char *argv[])
{
	int result;
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

	// Load arpg as game.
	initialize_arpg_interface(&engine.game);

	// Initialize engine.
	if (initialize_engine(&engine)) {
		run_engine(&engine);
		result = -1;
	}
	else {
		result = 0;

		// Pause at the end so we can read errors.
#if defined(_DEBUG)
		system("pause");
#endif
	}

	// Shut down.
	destroy_engine(&engine);

	return result;
}

