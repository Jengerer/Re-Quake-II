#ifndef _ENGINE_COMMON_H_
#define _ENGINE_COMMON_H_

#include "renderer_common.h"

// Structure for representing a shader program.
typedef struct engine_shader_program
{
	renderer_shader_t vertex;
	renderer_shader_t fragment;
	renderer_program_t program;
} engine_shader_program_t;

// Engine resource null initialization.
void engine_null_shader_program(engine_shader_program_t *program);

#endif // _ENGINE_COMMON_H_