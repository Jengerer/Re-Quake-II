#include "engine_common.h"

// Null an engine shader program for safe destruction.
void engine_null_shader_program(engine_shader_program_t *program)
{
	renderer_null_shader(&program->vertex);
	renderer_null_shader(&program->fragment);
	renderer_null_program(&program->program);
}
