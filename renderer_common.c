#include "renderer_common.h"
#include "memory_manager.h"

/* Null pointer to renderer shader. */
void renderer_null_shader(renderer_shader_t *shader)
{
	shader->buffer = NULL;
}

/* Null pointer to shader program. */
void renderer_null_program(renderer_program_t *program)
{
	program->buffer = NULL;
}

/* Null pointer to shader schema. */
void renderer_null_shader_schema(renderer_shader_schema_t *schema)
{
	schema->buffer = NULL;
}

/* Null pointer to uniform variable. */
void renderer_null_uniform(renderer_uniform_t *uniform)
{
	uniform->buffer = NULL;
}

/* Null pointer to model for safe cleanup. */
void renderer_null_model(renderer_model_t *model)
{
	model->buffer = NULL;
}
