#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include "renderer.h"
#include "opengl_common.h"

// Structure for representing an OpenGL texture.
typedef struct opengl_texture
{
	GLuint handle;
} opengl_texture_t;

// Representation of a mesh for rendering in OpenGL.
typedef struct opengl_model
{
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint array_size;
} opengl_model_t;

// Renderer/OpenGL conversions.
GLenum get_opengl_shader_type(renderer_shader_type_t type);

// Renderer initialization and clean-up.
int opengl_initialize(void);
void opengl_destroy(void);

// Renderer texture functions.
int opengl_create_texture2d(
	const image_t *image,
	renderer_texture_t *out);
void opengl_bind_texture2d(
	renderer_texture_t texture,
	renderer_uniform_t shader_texture);
void opengl_unbind_texture2d(void);

// Renderer model functions.
int opengl_create_model(
	const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);
int opengl_create_indexed_model(
	const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);
void opengl_destroy_model(renderer_model_t *model);
void opengl_draw_model(renderer_model_t model, renderer_shader_schema_t schema);

// Renderer drawing functions.
void opengl_clear_scene(void);

#endif // _OPENGL_RENDERER_H_