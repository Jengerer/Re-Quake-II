#include "opengl_export.h"
#include "opengl_renderer.h"

// Fill out structure with OpenGL renderer function implementations.
void opengl_populate_renderer(renderer_t *renderer)
{
	// Fill out interface functions.
	// Initialization and destruction.
	renderer->initialize = &opengl_initialize;
	renderer->destroy = &opengl_destroy;

	// Shader functions.
	renderer->create_shader_schema = &opengl_create_shader_schema;
	renderer->destroy_shader_schema = &opengl_destroy_shader_schema;
	renderer->create_shader = &opengl_create_shader;
	renderer->destroy_shader = &opengl_destroy_shader;

	// Shader program functions.
	renderer->create_program = &opengl_create_program;
	renderer->destroy_program = &opengl_destroy_program;
	renderer->link_shader = &opengl_link_shader;
	renderer->compile_program = &opengl_compile_program;
	renderer->set_program = &opengl_set_program;
	renderer->unset_program = &opengl_unset_program;

	// Shader attribute and variable functions.
	renderer->create_shader_schema = &opengl_create_shader_schema;
	renderer->destroy_shader_schema = &opengl_destroy_shader_schema;
	renderer->get_uniform = &opengl_get_uniform;
	renderer->destroy_uniform = &opengl_destroy_uniform;
	renderer->set_uniform_vector3d = &opengl_set_uniform_vector3d;
	renderer->set_uniform_vector4d = &opengl_set_uniform_vector4d;
	renderer->set_uniform_matrix3x3 = &opengl_set_uniform_matrix3x3;
	renderer->set_uniform_matrix4x4 = &opengl_set_uniform_matrix4x4;

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
