#ifndef _LICHT_GAME_H_
#define _LICHT_GAME_H_

#include "renderer_common.h"

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
	renderer_texture_t swag;
} client_context_t;

// Client context initialization.
void client_context_null(client_context_t *context);

// Game manager listener functions.
void client_initialize(void);
void client_shutdown(void);
void client_tick_begin(void);
void client_tick(void);
void client_tick_end(void);

#endif // _LICHT_GAME_H_
