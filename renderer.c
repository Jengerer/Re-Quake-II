#include "renderer.h"
#include <stdlib.h>

/*
 * Null base renderer interface for safe cleanup.
 */
void renderer_null_interface(renderer_t *renderer)
{
	// Initialization and clean-up functions.
	renderer->initialize = NULL;
	renderer->destroy = NULL;

	// Shader functions.
	renderer->create_shader = NULL;
	renderer->destroy_shader = NULL;

	// Shader program functions.
	renderer->create_program = NULL;
	renderer->destroy_program = NULL;
	renderer->link_shader = NULL;
	renderer->compile_program = NULL;
	renderer->set_program = NULL;
	renderer->unset_program = NULL;

	// Shader attribute and variable functions.
	renderer->create_shader_schema = NULL;
	renderer->destroy_shader_schema = NULL;
	renderer->get_uniform = NULL;
	renderer->destroy_uniform = NULL;
	renderer->set_uniform_vector3d = NULL;

	// Model functions.
	renderer->create_model = NULL;
	renderer->create_indexed_model = NULL;
	renderer->destroy_model = NULL;
	renderer->draw_model = NULL;

	// Rendering functions.
	renderer->clear_scene = NULL;
}

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
