#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include "renderer_shared.h"
#include "game_manager_utilities.h"

// Licht game state to test engine.
typedef struct client_context
{
	// Rendering parameters.
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_program_t program;
	renderer_shader_schema_t schema;
	renderer_uniform_t object;
	renderer_uniform_t view;
	renderer_uniform_t projection;
	renderer_uniform_t texture2d;

	// Utilities for game manager.
	game_manager_utilities_t game_manager;
} client_context_t;

// Game manager listener functions.
void client_null(void);
void client_initialize(void);
void client_shutdown(void);
void client_tick_begin(void);
void client_tick(void);
void client_tick_end(void);

// Client singleton.
client_context_t client;

#endif // _LICHT_GAME_H_
