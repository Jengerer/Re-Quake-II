#include "opengl_model.h"

/*
 * Null model for save destruction.
 */
void null_opengl_model(opengl_model_t *model)
{
	model->vertex_array = 0;
	model->vertex_buffer = 0;
	model->index_buffer = 0;
}
