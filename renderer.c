#include "renderer.h"
#include <stdlib.h>

/*
 * Null base renderer interface for safe cleanup.
 */
void null_renderer(renderer_t *renderer)
{
	renderer->initialize = NULL;
	renderer->destroy = NULL;
	renderer->create_model = NULL;
	renderer->create_indexed_model = NULL;
	renderer->destroy_model = NULL;
	renderer->clear_scene = NULL;
	renderer->render_model = NULL;
	renderer->create_shader = NULL;
	renderer->destroy_shader = NULL;
	renderer->create_shader_program = NULL;
	renderer->destroy_shader_program = NULL;
	renderer->set_shader_program = NULL;
	renderer->unset_shader_program = NULL;
}

/*
 * Null pointer to model for safe cleanup.
 */
void null_renderer_model(renderer_model_t *model)
{
	model->reference.as_pointer = NULL;
}
