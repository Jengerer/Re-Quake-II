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
	GLuint index;
	GLint num_floats;
} opengl_shader_attribute_t;

// Structure for representing an OpenGL shader schema.
typedef struct opengl_shader_schema
{
	GLsizei vertex_size;
	opengl_shader_attribute_t *attributes;
	int num_attributes;
} opengl_shader_schema_t;

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
int opengl_create_shader_schema(renderer_program_t program,
	renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out);
void opengl_destroy_shader_schema(renderer_shader_schema_t *schema);

// Renderer model functions.
int opengl_create_model(const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);
int opengl_create_indexed_model(const void *vertex_data,
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