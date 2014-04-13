#ifndef _PLATFORMER_H_
#define _PLATFORMER_H_

#include "aabb.h"
#include "game.h"
#include "map.h"

// Platformer game state to test engine.
typedef struct platformer_context
{
	aabb_t player;
	map_t map;
} platformer_context_t;

// Platformer context initialization.
void null_platformer_context(platformer_context_t *context);

// Fill out game interface.
void initialize_platformer_interface(game_t *game);

// Game interface functions.
const char* get_platformer_name();
int initialize_platformer(game_context_t **out);
void destroy_platformer(game_context_t *context);
int load_platformer_resources(game_context_t *context, renderer_t *renderer);
void free_platformer_resources(game_context_t *context, renderer_t *renderer);
int render_platformer(game_context_t *context, renderer_t *renderer);

#endif // _PLATFORMER_H_