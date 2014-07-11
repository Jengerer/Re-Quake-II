#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "indexed_mesh.h"

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

// Component to render in a scene.
typedef struct renderer_model
{
	void *buffer;
} renderer_model_t;

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

// Renderer self-initialization function.
typedef int (*renderer_initialize_fn)(void);

// Renderer shut-down function.
typedef void (*renderer_destroy_fn)(void);

// Loading and compiling a single shader.
typedef int (*renderer_create_shader_fn)(
	const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);

// Unlinking and cleaning up a shader.
typedef void (*renderer_destroy_shader_fn)(
	renderer_shader_t *shader,
	renderer_program_t program);

// Creating a full shader program.
typedef int (*renderer_create_program_fn)(
	renderer_shader_schema_t schema,
	renderer_program_t *out);

// Destroying a full shader program.
typedef void (*renderer_destroy_program_fn)(renderer_program_t *program);

// Link a shader to the program.
typedef void (*renderer_link_shader_fn)(
	renderer_shader_t shader,
	renderer_program_t program);

// Compile a shader program after shaders have been linked.
typedef int (*renderer_compile_program_fn)(renderer_program_t program);

// Set the active shader program used in rendering.
typedef void (*renderer_set_program_fn)(renderer_program_t program);

// Unset the active shader program.
typedef void (*renderer_unset_program_fn)(void);

// Generating a renderer-specific shader schema reference.
typedef int(*renderer_create_shader_schema_fn)(
	renderer_program_t program,
	renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out);

// Clean up renderer-specific shader schema.
typedef void(*renderer_destroy_shader_schema_fn)(renderer_shader_schema_t schema);

// Creating an unindexed renderable model.
typedef int (*renderer_create_model_fn)(
	const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);

// Creating an indexed vertex renderable model.
typedef int (*renderer_create_indexed_model_fn)(
	const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);

// Cleaning a generic model.
typedef void (*renderer_destroy_model_fn)(renderer_model_t *model);

// Drawing a model.
typedef void (*renderer_draw_model_fn)(
	renderer_model_t model,
	renderer_shader_schema_t schema);

// Clearing previous frame render.
typedef void (*renderer_clear_scene_fn)(void);

// Generic renderer interface struct.
typedef struct renderer
{
	// Initialization and clean-up functions.
	renderer_initialize_fn initialize;
	renderer_destroy_fn destroy;

	// Shader functions.
	renderer_create_shader_fn create_shader;
	renderer_destroy_shader_fn destroy_shader;

	// Shader program functions.
	renderer_create_program_fn create_program;
	renderer_destroy_program_fn destroy_program;
	renderer_link_shader_fn link_shader;
	renderer_compile_program_fn compile_program;
	renderer_set_program_fn set_program;
	renderer_unset_program_fn unset_program;
	renderer_create_shader_schema_fn create_shader_schema;
	renderer_destroy_shader_schema_fn destroy_shader_schema;

	// Model functions.
	renderer_create_model_fn create_model;
	renderer_create_indexed_model_fn create_indexed_model;
	renderer_destroy_model_fn destroy_model;
	renderer_draw_model_fn draw_model;

	// Rendering functions.
	renderer_clear_scene_fn clear_scene;
} renderer_t;

// Null renderer state for cleanup.
void renderer_null_interface(renderer_t *renderer);
void renderer_null_shader(renderer_shader_t *renderer);
void renderer_null_program(renderer_program_t *program);
void renderer_null_shader_schema(renderer_shader_schema_t *schema);
void renderer_null_model(renderer_shader_schema_t *schema);

#endif // _RENDERER_H_ 
