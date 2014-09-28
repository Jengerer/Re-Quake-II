#ifndef _RENDERER_COMMON_H_
#define _RENDERER_COMMON_H_

// Generic shader types.
typedef enum renderer_shader_type
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
} renderer_shader_type_t;

// Generic renderer variable types.
typedef enum renderer_variable_type
{
	VARIABLE_FLOAT,
	VARIABLE_VERTEX_2D,
	VARIABLE_VERTEX_3D,
	VARIABLE_VERTEX_4D,
	VARIABLE_MATRIX_2X2,
	VARIABLE_MATRIX_3X3,
	VARIABLE_MATRIX_4X4
} renderer_variable_type_t;

// Shader attribute definition.
typedef struct renderer_shader_attribute
{
	const char *name;
	renderer_variable_type_t type;
} renderer_shader_attribute_t;

// Schema for a shader program.
typedef struct renderer_shader_schema
{
	void *buffer;
} renderer_shader_schema_t;

// Single compiled shader.
typedef struct renderer_shader
{
	void *buffer;
} renderer_shader_t;

// Full shader program.
typedef struct renderer_program
{
	void *buffer;
} renderer_program_t;

// Shader uniform variable.
typedef struct renderer_uniform
{
	void *buffer;
} renderer_uniform_t;

// TWo dimensional texture.
typedef struct renderer_texture
{
	void *buffer;
} renderer_texture_t;

// Component to render in a scene.
typedef struct renderer_model
{
	void *buffer;
} renderer_model_t;

// Renderer common object initialization.
void renderer_null_shader(renderer_shader_t *shader);
void renderer_null_program(renderer_program_t *program);
void renderer_null_shader_schema(renderer_shader_schema_t *schema);
void renderer_null_uniform(renderer_uniform_t *uniform);
void renderer_null_model(renderer_model_t *schema);

#endif // _RENDERER_COMMON_H_