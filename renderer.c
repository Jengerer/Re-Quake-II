#include "renderer.h"
#include <stdlib.h>

/*
 * Null base renderer interface for safe cleanup.
 */
void null_renderer(renderer_t *renderer)
{
	renderer->context = NULL;
}

/*
 * Null pointer to model for safe cleanup.
 */
void null_renderer_model(renderer_model_t **model_ptr)
{
	*model_ptr = NULL;
}