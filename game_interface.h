#ifndef _GAME_INTERFACE_H_
#define _GAME_INTERFACE_H_

#include "renderer_interface.h"
#include "keyboard_manager.h"

// Function types for game interface.
typedef const char* (*game_get_name_t)(void);
typedef int (*game_initialize_t)(void);
typedef void (*game_destroy_t)(void);
typedef int (*game_render_t)(void);
typedef int (*game_frame_begin_t)(float time);
typedef int (*game_frame_end_t)(void);
typedef void (*game_handle_keyboard_t)(const keyboard_manager_t *keyboard);

// Structure for representing an interface to a game implementation.
typedef struct game_interface
{
	// Game initialization/destruction interface functions.
	game_get_name_t get_name;
	game_initialize_t initialize;
	game_destroy_t destroy;

	// Graphics interface functions.
	game_render_t render;

	// Frame running functions.
	game_frame_begin_t frame_begin;
	game_frame_end_t frame_end;

	// Input handling functions.
	game_handle_keyboard_t handle_keyboard;
} game_interface_t;

// Null game context.
void game_interface_null(game_interface_t *game);

#endif // _GAME_INTERFACE_H_
