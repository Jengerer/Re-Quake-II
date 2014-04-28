#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "indexed_mesh.h"

// Generic shader types.
typedef enum renderer_shader_type
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
} renderer_shader_type_t;

// Generic shader attribute types.
typedef enum renderer_shader_attribute_type
{
	ATTRIBUTE_FLOAT,
	ATTRIBUTE_TEXTURE_COORDINATE,
	ATTRIBUTE_VERTEX_2D,
	ATTRIBUTE_VERTEX_3D,
	ATTRIBUTE_VERTEX_4D
} renderer_shader_attribute_type_t;

// Shader attribute definition.
typedef struct renderer_shader_attribute
{
	const char *name;
	renderer_shader_attribute_type_t type;
} renderer_shader_attribute_t;

// Schema for a shader program.
typedef struct renderer_shader_schema
{
	const renderer_shader_attribute_t *attributes;
	int num_attributes;
} renderer_shader_schema_t;

// Union for minimum model/shader type.
typedef union renderer_reference
{
	unsigned int as_integer;
	void *as_pointer;
} renderer_reference_t;

// Component to render in a scene.
typedef struct renderer_model
{
	renderer_reference_t reference;
} renderer_model_t;

// Single compiled shader.
typedef struct renderer_shader
{
	renderer_reference_t reference;
} renderer_shader_t;

// Full shader program.
typedef struct renderer_shader_program
{
	renderer_reference_t reference;
	const renderer_shader_schema_t *schema;
} renderer_shader_program_t;

// Function types for renderer interface.
typedef int (*renderer_initialize_fn)(void);
typedef void (*renderer_destroy_fn)(void);
typedef int (*renderer_create_vertex_buffer_fn)(const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t *schema,
	renderer_model_t *out);
typedef int (*renderer_create_indexed_buffer_fn)(const void *vertex_data,
	int num_vertices,
	const void *index_data,
	int num_indices,
	const renderer_shader_schema_t *schema,
	renderer_model_t *out);
typedef void (*renderer_destroy_model_fn)(renderer_model_t *model);
typedef void (*renderer_clear_scene_fn)(void);
typedef void (*renderer_render_model_fn)(const renderer_model_t *model);
typedef int (*renderer_create_shader_program_fn)(renderer_shader_program_t *out, const renderer_shader_schema_t *schema);
typedef void (*renderer_destroy_shader_program_fn)(renderer_shader_program_t *program);
typedef int (*renderer_create_shader_fn)(const char *filename, renderer_shader_type_t type, renderer_shader_t *out);
typedef void (*renderer_destroy_shader_fn)(renderer_shader_t *shader, renderer_shader_program_t *program);
typedef void (*renderer_link_shader_fn)(renderer_shader_t shader, renderer_shader_program_t *program);
typedef int (*renderer_compile_shader_program_fn)(renderer_shader_program_t *program);
typedef void (*renderer_set_shader_program_fn)(renderer_shader_program_t *program);
typedef void (*renderer_unset_shader_program_fn)(void);

// Generic renderer interface struct.
typedef struct renderer
{
	// Interface functions pointers.
	renderer_initialize_fn initialize;
	renderer_destroy_fn destroy;
	renderer_create_vertex_buffer_fn create_model;
	renderer_create_indexed_buffer_fn create_indexed_model;
	renderer_destroy_model_fn destroy_model;
	renderer_clear_scene_fn clear_scene;
	renderer_render_model_fn render_model;
	renderer_create_shader_program_fn create_shader_program;
	renderer_destroy_shader_program_fn destroy_shader_program;
	renderer_create_shader_fn create_shader;
	renderer_destroy_shader_fn destroy_shader;
	renderer_link_shader_fn link_shader;
	renderer_compile_shader_program_fn compile_shader_program;
	renderer_set_shader_program_fn set_shader_program;
	renderer_unset_shader_program_fn unset_shader_program;
} renderer_t;

// Null renderer state for cleanup.
void null_renderer(renderer_t *renderer);
void null_renderer_model(renderer_model_t *model);

#endif // _RENDERER_H_ 
