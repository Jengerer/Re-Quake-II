#pragma once

// Generic shader types.
enum RendererShaderType
{
	VertexShader,
	FragmentShader
};

// Generic renderer variable types.
enum RendererVariableType
{
	FloatType,
	Vector2Type,
	Vector3Type,
	Vector4Type,
	Matrix2x2Type,
	Matrix3x3Type,
	Matrix4x4Type
};

// Shader attribute definition.
struct RendererShaderAttribute
{
	const char *name;
	RendererVariableType type;
};

// Schema for a shader program.
struct RendererShaderSchema
{
	void *buffer;
};

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

#endif // _RENDERER_SHARED_H_