#ifndef _OPENGL_SHADER_UTILITIES_H_
#define _OPENGL_SHADER_UTILITIES_H_

#include "opengl_common.h"
#include "vector3d.h"
#include "vector4d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

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

// Shader vertex attribute functions.
int opengl_create_shader_schema(
	renderer_program_t program,
	const renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out);
void opengl_destroy_shader_schema(renderer_shader_schema_t *schema);

// Shader uniform variables.
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

#endif // _OPENGL_SHADER_UTILITIES_H_