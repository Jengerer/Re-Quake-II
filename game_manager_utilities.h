#ifndef _GAME_MANAGER_UTILITIES_H_
#define _GAME_MANAGER_UTILITIES_H_

#include "renderer.h"
#include "renderer_shader_utilities.h"

// Request current time.
typedef float (*game_manager_get_time_t)(void);

// Renderer utilities.
typedef const renderer_t *(*game_manager_get_renderer_t)(void);
typedef const renderer_shader_utilities_t *(*game_manager_get_shader_utilities_t)(void);
typedef void (*game_manager_present_frame_t)(void);

// Interface for a game to request resources and perform tasks from the game manager.
typedef struct game_manager_utilities
{
	game_manager_get_time_t get_time;

	// Renderer utilities.
	game_manager_get_renderer_t get_renderer;
	game_manager_get_shader_utilities_t get_shader_utilities;
	game_manager_present_frame_t present_frame;
} game_manager_utilities_t;

#endif // _GAME_MANAGER_UTILITIES_H_