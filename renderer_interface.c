#include "renderer_interface.h"
#include "memory_manager.h"

/*
 * Null base renderer interface for safe cleanup.
 */
void renderer_null_interface(renderer_interface_t *renderer)
{
	// Initialization and clean-up functions.
	renderer->initialize = NULL;
	renderer->destroy = NULL;

	// Shader functions.
	renderer->create_shader = NULL;
	renderer->destroy_shader = NULL;

	// Shader program functions.
	renderer->create_program = NULL;
	renderer->destroy_program = NULL;
	renderer->link_shader = NULL;
	renderer->compile_program = NULL;
	renderer->set_program = NULL;
	renderer->unset_program = NULL;

	// Shader attribute and variable functions.
	renderer->create_shader_schema = NULL;
	renderer->destroy_shader_schema = NULL;
	renderer->get_uniform = NULL;
	renderer->destroy_uniform = NULL;
	renderer->set_uniform_vector3d = NULL;
	renderer->set_uniform_vector4d = NULL;
	renderer->set_uniform_matrix3x3 = NULL;
	renderer->set_uniform_matrix4x4 = NULL;
	renderer->set_uniform_integer = NULL;

	// Texture functions.
	renderer->create_texture2d = NULL;
	renderer->bind_texture2d = NULL;
	renderer->unbind_texture2d = NULL;

	// Model functions.
	renderer->create_model = NULL;
	renderer->create_indexed_model = NULL;
	renderer->destroy_model = NULL;
	renderer->draw_model = NULL;

	// Rendering functions.
	renderer->clear_scene = NULL;
}
