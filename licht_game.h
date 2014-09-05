#ifndef _LICHT_GAME_H_
#define _LICHT_GAME_H_

#include "camera.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "renderable_object.h"
#include "world.h"

// Licht game state to test engine.
typedef struct licht_context
{
	camera_t camera;
	player_t player;
	world_t world;

	// Rendering world.
	renderable_object_t *renderable_head;

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
} licht_context_t;

// Licht context initialization.
void licht_context_null(licht_context_t *context);

// Fill out game interface.
void licht_initialize_interface(game_t *game);

// Game interface functions.
const char* licht_get_name(void);
int licht_initialize(void);
void licht_destroy(void);
int licht_load_resources(const renderer_t *renderer);
void licht_free_resources(const renderer_t *renderer);
int licht_render(const renderer_t *renderer);
int licht_frame_begin(float time);
int licht_frame_end(void);
void licht_handle_keyboard(const keyboard_manager_t *keyboard);

#endif // _LICHT_GAME_H_
