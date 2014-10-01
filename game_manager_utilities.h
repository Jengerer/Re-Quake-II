#ifndef _GAME_MANAGER_UTILITIES_H_
#define _GAME_MANAGER_UTILITIES_H_

#include "game_manager_shared.h"
#include "renderer_shared.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "vector3d.h"
#include "vector4d.h"

// Request current time.
typedef float (*game_manager_get_time_t)(void);

// Renderer utilities.
typedef void (*game_manager_clear_frame_t)(void);
typedef void (*game_manager_present_frame_t)(void);

// Shader utilities.
typedef void (*game_manager_create_shader_t)(const char *filename, renderer_shader_t *out);
typedef void (*game_manager_create_shader_program_t)(
	renderer_shader_t vertex_shader,
	renderer_shader_t fragment_shader,
	renderer_program_t *out);
typedef void (*game_manager_set_program_t)(renderer_program_t program);
typedef void (*game_manager_get_uniform_t)(
	renderer_program_t program,
	const char *name,
	renderer_uniform_t *out);
typedef void (*game_manager_set_uniform_matrix3x3_t)(
	renderer_uniform_t uniform,
	const matrix3x3_t *matrix);
typedef void (*game_manager_set_uniform_matrix4x4_t)(
	renderer_uniform_t uniform,
	const matrix4x4_t *matrix);

// Interface for a game to request resources and perform tasks from the game manager.
typedef struct game_manager_utilities
{
	game_manager_get_time_t get_time;

	// Rendering utilities.
	game_manager_clear_frame_t clear_frame;
	game_manager_present_frame_t present_frame;

	// Shader utilities.
	game_manager_create_shader_t create_shader;
	game_manager_create_shader_program_t create_shader_program;
	game_manager_set_program_t set_program;

	// Shader uniform variable functions.
	game_manager_get_uniform_t get_uniform;
	game_manager_set_uniform_matrix3x3_t set_uniform_matrix3x3;
	game_manager_set_uniform_matrix4x4_t set_uniform_matrix4x4;
} game_manager_utilities_t;

#endif // _GAME_MANAGER_UTILITIES_H_