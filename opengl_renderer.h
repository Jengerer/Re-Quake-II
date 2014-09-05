#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>
#include "polygon.h"
#include "renderer.h"

// Structure representing global OpenGL state.
typedef struct opengl_context
{
	GLuint active_program;
} opengl_context_t;

// Structure for representing an OpenGL shader.
typedef struct opengl_shader
{
	GLuint handle;
	char is_linked;
} opengl_shader_t;

// Structure for representing an OpenGL shader program.
typedef struct opengl_program
{
	GLuint handle;
} opengl_program_t;

// Structure for representing an OpenGL shader attribute.
typedef struct opengl_shader_attribute
{
	GLint location;
	GLint num_floats;
	GLchar* offset;
} opengl_shader_attribute_t;

// Structure for representing an OpenGL shader schema.
typedef struct opengl_shader_schema
{
	GLsizei vertex_size;
	opengl_shader_attribute_t *attributes;
	int num_attributes;
} opengl_shader_schema_t;

// Structure for representing a uniform variable.
typedef struct opengl_uniform
{
	GLint location;
} opengl_uniform_t;

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

// Structure nulling functions.
void opengl_null_context(opengl_context_t *state);
void opengl_null_shader(opengl_shader_t *shader);
void opengl_null_program(opengl_program_t *program);
void opengl_null_shader_schema(opengl_shader_schema_t *schema);
void opengl_null_uniform(opengl_uniform_t *uniform);
void opengl_null_texture(opengl_texture_t *texture);
void opengl_null_model(opengl_model_t *model);

// Renderer/OpenGL conversions.
GLenum get_opengl_shader_type(renderer_shader_type_t type);

// Filling out renderer interface.
void initialize_opengl_interface(renderer_t *renderer);

// Renderer initialization and clean-up.
int opengl_initialize(void);
void opengl_destroy(void);

// Renderer shader functions.
int opengl_create_shader(const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);
void opengl_destroy_shader(renderer_shader_t *shader, renderer_program_t program);

// Renderer shader program functions.
int opengl_create_program(renderer_program_t *out);
void opengl_destroy_program(renderer_program_t *out);
void opengl_link_shader(renderer_shader_t shader, renderer_program_t program);
int opengl_compile_program(renderer_program_t program);
void opengl_set_program(renderer_program_t program);
void opengl_unset_program(void);

// Shader attribute and variable functions.
int opengl_create_shader_schema(
	renderer_program_t program,
	const renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out);
void opengl_destroy_shader_schema(renderer_shader_schema_t *schema);
int opengl_get_uniform(
	renderer_program_t program,
	const char *name,
	renderer_uniform_t *out);
void opengl_destroy_uniform(renderer_uniform_t *out);
void opengl_set_uniform_vector3d(
	renderer_uniform_t uniform,
	const vector3d_t *vector);
void opengl_set_uniform_vector4d(
	renderer_uniform_t uniform,
	const vector4d_t *vector);
void opengl_set_uniform_matrix3x3(
	renderer_uniform_t uniform,
	const matrix3x3_t *matrix);
void opengl_set_uniform_matrix4x4(
	renderer_uniform_t uniform,
	const matrix4x4_t *matrix);
void opengl_set_uniform_integer(
	renderer_uniform_t uniform,
	int value);

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