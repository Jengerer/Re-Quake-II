#include "engine_export.h"
#include "error_stack.h"
#include "game_manager_export.h"
#include "opengl_export.h"
#include "client_export.h"
#include <stdio.h>
#include <stdlib.h>

// Application constants.
#define APPLICATION_WIDTH 1024
#define APPLICATION_HEIGHT 768

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// Engine references and interfaces.
	engine_interface_t engine;
	engine_utilities_t *engine_utilities;
	engine_listener_t *engine_listener;
	renderer_t *renderer;
	renderer_shader_utilities_t *shaders;

	// Game manager references and interfaces.
	game_manager_utilities_t *game_manager_utilities;

	// Client game references.
	game_manager_listener_t *client_listener;

	// Let's do it!
	int success;

	// Get engine references.
	engine_listener = engine_get_listener_destination();
	renderer = engine_get_renderer_destination();
	shaders = engine_get_shader_utilities_destination();

	// Get game manager references.
	engine_utilities = game_manager_get_engine_utilities_destination();
	client_listener = game_manager_get_client_listener_destination();

	// Get client reference.
	game_manager_utilities = client_get_game_manager_utilities_destination();

	// Get engine to fill out.
	engine_populate_interface(&engine);
	engine_populate_utilities(engine_utilities);

	// Get renderer to fill out.
	populate_renderer(renderer);
	populate_renderer_shader_utilities(shaders);

	// Get game manager to fill out.
	game_manager_populate_engine_listener(engine_listener);
	game_manager_populate_utilities(game_manager_utilities);

	// Get client to fill out.
	client_populate_game_manager_listener(client_listener);

	// Initialize error stack for error logging..
	error_stack_initialize();

	// Initialize engine and run.
	success = engine.initialize() && engine.run();

	// How did we do?
	if (!success) {
		// Dump what went wrong.
		error_stack_dump();

		// Pause at the end so we can read errors.
#if defined(_DEBUG)
		system("pause");
#endif
	}

	engine.shutdown();
	return success;
}

