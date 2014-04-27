#ifndef _PLATFORMER_H_
#define _PLATFORMER_H_

#include "game.h"
#include "map.h"
#include "player.h"

// Platformer game state to test engine.
typedef struct platformer_context
{
	player_t player;
	map_t map;

	// Rendering parameters.
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_shader_program_t program;
} platformer_context_t;

// Platformer context initialization.
void null_platformer_context(platformer_context_t *context);

// Fill out game interface.
void initialize_platformer_interface(game_t *game);

// Game interface functions.
const char* get_platformer_name(void);
int initialize_platformer(void);
void destroy_platformer(void);
int load_platformer_resources(renderer_t *renderer);
void free_platformer_resources(renderer_t *renderer);
int render_platformer(renderer_t *renderer);
void handle_platformer_keyboard(keyboard_manager_t *keyboard);

#endif // _PLATFORMER_H_
