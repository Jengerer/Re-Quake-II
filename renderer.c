#include "renderer.h"
#include <stdlib.h>

/*
 * Null base renderer interface for safe cleanup.
 */
void null_renderer(renderer_t *renderer)
{
	renderer->initialize = NULL;
	renderer->destroy = NULL;
	renderer->create_mesh_model = NULL;
	renderer->create_indexed_mesh_model = NULL;
	renderer->destroy_model = NULL;
	renderer->clear_scene = NULL;
	renderer->render_model = NULL;
	renderer->create_shader = NULL;
}

/*
 * Null pointer to model for safe cleanup.
 */
void null_renderer_model(renderer_model_t **model_ptr)
{
	*model_ptr = NULL;
}

