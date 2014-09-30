#ifndef _RENDERER_SHADER_UTILITIES_H_
#define _RENDERER_SHADER_UTILITIES_H_

#include "renderer_shared.h"
#include "vector3d.h"
#include "vector4d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

// Loading and compiling a single shader.
typedef int (*renderer_create_shader_t)(
	const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);

// Unlinking and cleaning up a shader.
typedef void (*renderer_destroy_shader_t)(
	renderer_shader_t *shader,
	renderer_program_t program);

// Creating a full shader program.
typedef int (*renderer_create_program_t)(renderer_program_t *out);

// Destroying a full shader program.
typedef void (*renderer_destroy_program_t)(renderer_program_t *program);

// Link a shader to the program.
typedef void (*renderer_link_shader_t)(
	renderer_shader_t shader,
	renderer_program_t program);

// Compile a shader program after shaders have been linked.
typedef int (*renderer_compile_program_t)(renderer_program_t program);

// Set the active shader program used in rendering.
typedef void (*renderer_set_program_t)(renderer_program_t program);

// Unset the active shader program.
typedef void (*renderer_unset_program_t)(void);

// Generating a renderer-specific shader schema reference.
typedef int (*renderer_create_shader_schema_t)(
	renderer_program_t program,
	const renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out);

// Clean up renderer-specific shader schema.
typedef void (*renderer_destroy_shader_schema_t)(renderer_shader_schema_t *schema);

// Gets a handle to a shader uniform variable.
typedef int (*renderer_get_uniform_t)(
	renderer_program_t program,
	const char *name,
	renderer_uniform_t *out);

// Destroy a handle to a shader uniform variable.
typedef void (*renderer_destroy_uniform_t)(renderer_uniform_t *out);

// Sets a 3D vector value for a uniform variable.
typedef void (*renderer_set_uniform_vector3d_t)(
	renderer_uniform_t uniform,
	const vector3d_t *vector);

// Sets a 4D vector value for a uniform variable.
typedef void (*renderer_set_uniform_vector4d_t)(
	renderer_uniform_t uniform,
	const vector4d_t *vector);

// Sets up a 3x3 matrix value for a uniform variable.
typedef void (*renderer_set_uniform_matrix3x3_t)(
	renderer_uniform_t uniform,
	const matrix3x3_t *matrix);

// Sets up 4x4 matrix value for a uniform variable.
typedef void (*renderer_set_uniform_matrix4x4_t)(
	renderer_uniform_t uniform,
	const matrix4x4_t *matrix);

// Set up integer uniform variable.
typedef void (*renderer_set_uniform_integer_t)(
	renderer_uniform_t uniform,
	int value);

// Interface for managing rendering shaders.
typedef struct renderer_shader_utilities
{
	// Shader functions.
	renderer_create_shader_t create_shader;
	renderer_destroy_shader_t destroy_shader;

	// Shader program functions.
	renderer_create_program_t create_program;
	renderer_destroy_program_t destroy_program;
	renderer_link_shader_t link_shader;
	renderer_compile_program_t compile_program;
	renderer_set_program_t set_program;
	renderer_unset_program_t unset_program;

	// Shader vertex attribute functions.
	renderer_create_shader_schema_t create_shader_schema;
	renderer_destroy_shader_schema_t destroy_shader_schema;

	// Shader uniform variable functions.
	renderer_get_uniform_t get_uniform;
	renderer_destroy_uniform_t destroy_uniform;
	renderer_set_uniform_vector3d_t set_uniform_vector3d;
	renderer_set_uniform_vector4d_t set_uniform_vector4d;
	renderer_set_uniform_matrix3x3_t set_uniform_matrix3x3;
	renderer_set_uniform_matrix4x4_t set_uniform_matrix4x4;
	renderer_set_uniform_integer_t set_uniform_integer;
} renderer_shader_utilities_t;

#endif // _SHADER_UTILITIES_H_