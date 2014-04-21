#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "indexed_mesh.h"

// Generic renderer types.
typedef void renderer_model_t;
typedef void renderer_shader_t;

// Generic shader types.
typedef enum renderer_shader_type
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
} renderer_shader_type_t;

// Function types for renderer interface.
typedef int (*renderer_initialize_fn)();
typedef void (*renderer_destroy_fn)();
typedef int (*renderer_create_mesh_model_fn)(const mesh_t* mesh, renderer_model_t **out);
typedef int (*renderer_create_indexed_mesh_model_fn)(const indexed_mesh_t* mesh, renderer_model_t **out);
typedef void (*renderer_destroy_model_fn)(renderer_model_t *model);
typedef void (*renderer_render_model_fn)(const renderer_model_t *model);
typedef int (*renderer_create_shader_fn)(const char *filename, renderer_shader_type_t type, renderer_shader_t *out);

// Generic renderer interface struct.
typedef struct renderer
{
	// Interface functions pointers.
	renderer_initialize_fn initialize;
	renderer_destroy_fn destroy;
	renderer_create_mesh_model_fn create_mesh_model;
	renderer_create_indexed_mesh_model_fn create_indexed_mesh_model;
	renderer_destroy_model_fn destroy_model;
	renderer_render_model_fn render_model;
	renderer_create_shader_fn create_shader;
} renderer_t;

// Null renderer state for cleanup.
void null_renderer(renderer_t *renderer);
void null_renderer_model(renderer_model_t **model_ptr);

#endif // _RENDERER_H_ 
