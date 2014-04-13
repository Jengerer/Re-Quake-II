#include "opengl_model.h"

/*
 * Null model for save destruction.
 */
void null_opengl_model(opengl_model_t *model)
{
	model->vertex_buffer = 0;
}
