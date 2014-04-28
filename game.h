#ifndef _GAME_H_
#define _GAME_H_

#include "renderer.h"
#include "keyboard_manager.h"

// Function types for game interface.
typedef const char* (*game_get_name_fn)(void);
typedef int (*game_initialize_fn)(void);
typedef void (*game_destroy_fn)(void);
typedef int (*game_load_resources_fn)(renderer_t *renderer);
typedef void (*game_free_resources_fn)(renderer_t *renderer);
typedef int (*game_render_fn)(renderer_t *renderer);
typedef void (*game_handle_keyboard_fn)(keyboard_manager_t *keyboard);

// Structure for representing an interface to a game implementation.
typedef struct game
{
	// Game initialization/destruction interface functions.
	game_get_name_fn get_name;
	game_initialize_fn initialize;
	game_destroy_fn destroy;

	// Graphics interface functions.
	game_load_resources_fn load_resources;
	game_free_resources_fn free_resources;
	game_render_fn render;

	// Input handling functions.
	game_handle_keyboard_fn handle_keyboard;
} game_t;

// Null game context.
void null_game(game_t *game);

#endif // _GAME_H_
