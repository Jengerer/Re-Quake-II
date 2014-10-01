#pragma once

#include "renderer_shared.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

// Interface for preparing shaders and uniform variables.
class RendererShadingResources
{
public:

	// Shader functions.

	// Loading and compiling a single shader.
	virtual RendererShader *CreateShader(const char *filename, RendererShaderType type) = 0;
	// Destroy/delete a shader.
	virtual void DestroyShader(RendererShader *shader) = 0;
	// Build a shader program.
	virtual RendererProgram *CreateProgram(const RendererShader *vertexShader, const RendererShader *fragmentShader) = 0;
	// Destroy a shader program.
	virtual void DestroyProgram(RendererProgram *program) = 0;
	// Prepare a shader schema for vertex attributes.
	virtual RendererShaderSchema *CreateShaderSchema(
		const RendererProgram *program,
		const RendererShaderAttribute *attributes,
		int numAttributes) = 0;
	// Destroy a shader schema.
	virtual void DestroyShaderSchema(RendererShaderSchema *schema) = 0;

	// Uniform variable functions.

	// Get uniform variable from a program.
	virtual RendererUniform *GetUniform(const RendererProgram *program, const char *name) = 0;
	// Destroy uniform variable.
	virtual void DestroyUniform(RendererUniform *uniform) = 0;

};

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