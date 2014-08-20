#ifndef _LICHT_GAME_H_
#define _LICHT_GAME_H_

#include "camera.h"
#include "game.h"
#include "map.h"
#include "player.h"

// Licht game state to test engine.
typedef struct licht_context
{
	camera_t camera;
	player_t player;
	map_t map;

	// Rendering parameters.
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_program_t program;
	renderer_shader_schema_t schema;
	renderer_uniform_t object;
	renderer_uniform_t view;
	renderer_uniform_t projection;
} licht_context_t;

// Licht context initialization.
void licht_context_null(licht_context_t *context);

// Fill out game interface.
void licht_initialize_interface(game_t *game);

// Game interface functions.
const char* licht_get_name(void);
int licht_initialize(void);
void licht_destroy(void);
int licht_load_resources(renderer_t *renderer);
void licht_free_resources(renderer_t *renderer);
int licht_render(renderer_t *renderer);
void licht_handle_keyboard(keyboard_manager_t *keyboard);

#endif // _LICHT_GAME_H_
