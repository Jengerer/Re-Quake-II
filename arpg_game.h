#ifndef _ARPG_GAME_H_
#define _ARPG_GAME_H_

#include "camera.h"
#include "game.h"
#include "map.h"
#include "player.h"

// ARPG game state to test engine.
typedef struct arpg_context
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
} arpg_context_t;

// ARPG context initialization.
void null_arpg_context(arpg_context_t *context);

// Fill out game interface.
void initialize_arpg_interface(game_t *game);

// Game interface functions.
const char* get_arpg_name(void);
int initialize_arpg(void);
void destroy_arpg(void);
int load_arpg_resources(renderer_t *renderer);
void free_arpg_resources(renderer_t *renderer);
int render_arpg(renderer_t *renderer);
void handle_arpg_keyboard(keyboard_manager_t *keyboard);

#endif // _ARPG_GAME_H_
