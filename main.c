#include "engine.h"
#include "error_stack.h"
#include "opengl_renderer.h"
#include "licht_game.h"
#include <stdio.h>

// Application constants.
#define APPLICATION_WIDTH 1024
#define APPLICATION_HEIGHT 768

int main(int argc, char *argv[])
{
	int success;
	(void)argc;
	(void)argv;

	// Engine state.
	engine_t engine;
	engine_null(&engine);

	// Create configuration for engine.
	engine_configuration_t *config = &engine.config;
	config->width = APPLICATION_WIDTH;
	config->height = APPLICATION_HEIGHT;

	// Initialize error stack for error logging..
	error_stack_initialize();

	// Set OpenGL as renderer.
	initialize_opengl_interface(&engine.renderer);

	// Load Licht as game.
	licht_initialize_interface(&engine.game);

	// Initialize engine and go!
	success = engine_initialize(&engine) && engine_run(&engine);
	engine_destroy(&engine);

	// How did we do?
	if (!success) {
		// Dump what went wrong.
		error_stack_dump();

		// Pause at the end so we can read errors.
#if defined(_DEBUG)
		system("pause");
#endif
	}

	return success;
}

