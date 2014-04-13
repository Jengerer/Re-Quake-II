#include "renderer.h"
#include <stdlib.h>

/*
 * Null base renderer interface for safe cleanup.
 */
void null_renderer(renderer_t *renderer)
{
	renderer->renderer_context = NULL;
}