#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "vector3d.h"
#include "vector4d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "image.h"
#include "renderer_shared.h"

// Renderer initialization and clean-up.
typedef void (*renderer_null_t)(void);
typedef int (*renderer_initialize_t)(void);
typedef void (*renderer_destroy_t)(void);

// Create a texture from an image.
typedef int (*renderer_create_texture2d_t)(
	const image_t *image,
	renderer_texture_t *out);

// Bind a texture to be rendered.
typedef void (*renderer_bind_texture2d_t)(
	renderer_texture_t texture,
	renderer_uniform_t shader_texture);

// Unbind the texture.
typedef void (*renderer_unbind_texture2d_t)(void);

// Creating an unindexed renderable model.
typedef int (*renderer_create_model_t)(
	const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);

// Creating an indexed vertex renderable model.
typedef int (*renderer_create_indexed_model_t)(
	const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);

// Cleaning a generic model.
typedef void (*renderer_destroy_model_t)(renderer_model_t *model);

// Drawing a model.
typedef void (*renderer_draw_model_t)(
	renderer_model_t model,
	renderer_shader_schema_t schema);

// Clearing previous frame render.
typedef void (*renderer_clear_scene_t)(void);

// Generic renderer interface struct.
typedef struct renderer
{
	// Initialization and clean-up functions.
	renderer_null_t null;
	renderer_initialize_t initialize;
	renderer_destroy_t destroy;

	// Texture functions.
	renderer_create_texture2d_t create_texture2d;
	renderer_bind_texture2d_t bind_texture2d;
	renderer_unbind_texture2d_t unbind_texture2d;

	// Model functions.
	renderer_create_model_t create_model;
	renderer_create_indexed_model_t create_indexed_model;
	renderer_destroy_model_t destroy_model;
	renderer_draw_model_t draw_model;

	// Rendering functions.
	renderer_clear_scene_t clear_scene;
} renderer_t;

#endif // _RENDERER_H_ 