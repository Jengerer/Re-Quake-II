#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "mesh.h"

// Generic renderer types.
typedef void renderer_context_t;
typedef void renderer_model_t;
typedef void renderer_shader_t;

// Generic shader types.
typedef enum renderer_shader_type
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
} renderer_shader_type_t;

// Function types for filling out the struct.
typedef int (*initialize_fn)(renderer_context_t **out);
typedef void (*destroy_fn)(renderer_context_t *context);
typedef int (*create_model_fn)(renderer_context_t *context, const mesh_t* mesh, renderer_model_t *model);
typedef void (*render_model_fn)(renderer_context_t *context, const renderer_model_t *model);
typedef int (*create_shader_fn)(renderer_context_t *context, const char *filename, renderer_shader_type_t type, renderer_shader_t *out);

// Generic renderer interface struct.
typedef struct renderer
{
	renderer_context_t *renderer_context;

	// Interface functions.
	initialize_fn initialize;
	destroy_fn destroy;
	create_model_fn create_model;
	render_model_fn render_model;
	create_shader_fn create_shader;
} renderer_t;

// Null renderer state for cleanup.
void null_renderer(renderer_t *renderer);

#endif // _RENDERER_H_ 