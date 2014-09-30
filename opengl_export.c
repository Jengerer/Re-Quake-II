#include "opengl_export.h"
#include "opengl_renderer.h"
#include "opengl_shader_utilities.h"

// Fill out structure with OpenGL renderer function implementations.
void populate_renderer(renderer_t *renderer)
{
	// Fill out interface functions.
	// Initialization and destruction.
	renderer->initialize = &opengl_initialize;
	renderer->destroy = &opengl_destroy;

	// Texture functions.
	renderer->create_texture2d = &opengl_create_texture2d;
	renderer->bind_texture2d = &opengl_bind_texture2d;
	renderer->unbind_texture2d = &opengl_unbind_texture2d;

	// Model functions.
	renderer->create_model = &opengl_create_model;
	renderer->create_indexed_model = &opengl_create_indexed_model;
	renderer->destroy_model = &opengl_destroy_model;
	renderer->draw_model = &opengl_draw_model;

	// Rendering functions.
	renderer->clear_scene = &opengl_clear_scene;
}

// Fill out structure for OpenGL shader utilities.
void populate_renderer_shader_utilities(renderer_shader_utilities_t *utilities)
{
	// Shader initialization.
	utilities->create_shader = &opengl_create_shader;
	utilities->destroy_shader = &opengl_destroy_shader;

	// Shader program functions.
	utilities->create_program = &opengl_create_program;
	utilities->destroy_program = &opengl_destroy_program;
	utilities->link_shader = &opengl_link_shader;
	utilities->compile_program = &opengl_compile_program;
	utilities->set_program = &opengl_set_program;
	utilities->unset_program = &opengl_unset_program;

	// Shader attribute and variable functions.
	utilities->create_shader_schema = &opengl_create_shader_schema;
	utilities->destroy_shader_schema = &opengl_destroy_shader_schema;
	utilities->get_uniform = &opengl_get_uniform;
	utilities->destroy_uniform = &opengl_destroy_uniform;
	utilities->set_uniform_vector3d = &opengl_set_uniform_vector3d;
	utilities->set_uniform_vector4d = &opengl_set_uniform_vector4d;
	utilities->set_uniform_matrix3x3 = &opengl_set_uniform_matrix3x3;
	utilities->set_uniform_matrix4x4 = &opengl_set_uniform_matrix4x4;
}