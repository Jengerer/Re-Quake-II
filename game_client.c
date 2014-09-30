#include "game_client.h"
#include "math_common.h"
#include "memory_manager.h"
#include <stdio.h>

// Rendering parameters.
#define NUM_PERSPECTIVE_SHADER_ATTRIBUTES 2
#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"
#define NEAR_DISTANCE 1.0f
#define FAR_DISTANCE 1024.0f

// Singleton game context.
client_context_t client;

// Renderer shader schemas.
const renderer_shader_attribute_t mesh_attributes[NUM_PERSPECTIVE_SHADER_ATTRIBUTES] = {
	{"in_vertex", VARIABLE_VERTEX_3D},
	{"in_texture", VARIABLE_VERTEX_2D}
};

// Private functions.
static void client_load_resources(void);
static void client_free_resources(void);
static void client_initialize_shaders(void);

// Null licht context for safe destruction.
void client_null(void)
{
	// Null rendering parameters.
	renderer_null_shader(&client.vertex_shader);
	renderer_null_shader(&client.fragment_shader);
	renderer_null_program(&client.program);
	renderer_null_shader_schema(&client.schema);
	renderer_null_uniform(&client.object);
	renderer_null_uniform(&client.view);
	renderer_null_uniform(&client.projection);
	renderer_null_uniform(&client.texture2d);
}

// Initialize client game module.
void client_initialize(void)
{
	client_load_resources();
}

// Destroy the licht context.
void client_destroy(void)
{
	client_free_resources();
}

// Load all base resources required by client.
void client_load_resources(void)
{
	const renderer_shader_utilities_t *shaders;
	matrix4x4_t perspective_matrix;

	client_initialize_shaders();

	// Set up the program for retrieving the uniforms.
	shaders = client.game_manager.get_shader_utilities();
	shaders->set_program(client.program);

	// Get the location to the transform and projection matrix.
	// TODO: Definitely need game manager wrapper on renderer/shader for error handling.
	shaders->get_uniform(client.program, "object", &client.object);
	//shaders->get_uniform(client.program, "view", &client.view);
	shaders->get_uniform(client.program, "projection", &client.projection);
	//shaders->get_uniform(client.program, "texture2d", &client.texture2d);

	// Generate projection matrix.
	matrix4x4_perspective(4.0f / 3.0f, 90.0f, NEAR_DISTANCE, FAR_DISTANCE, &perspective_matrix);
	shaders->set_uniform_matrix4x4(client.projection, &perspective_matrix);

	// Unset program.
	shaders->unset_program();
}

// Free base resources for client.
void client_free_resources(void)
{
	const renderer_shader_utilities_t *shaders;
	
	// Free uniforms.
	shaders = client.game_manager.get_shader_utilities();
	shaders->destroy_uniform(&client.object);
	shaders->destroy_uniform(&client.view);
	shaders->destroy_uniform(&client.projection);

	// Destroy schema.
	shaders->destroy_shader_schema(&client.schema);

	// Destroy program and shaders.
	shaders->destroy_shader(&client.vertex_shader, client.program);
	shaders->destroy_shader(&client.fragment_shader, client.program);
	shaders->destroy_program(&client.program);
}

// Game rendering function.
void client_render(void)
{
	const renderer_t *renderer;
	const renderer_shader_utilities_t *shaders;

	// Retrieve interfaces.
	renderer = client.game_manager.get_renderer();	
	shaders = client.game_manager.get_shader_utilities();
	
	// Clear the scene.
	renderer->clear_scene();

	// Set up the program.
	shaders->set_program(client.program);	
}

// Handle pre-frame event.
void client_tick_begin(void)
{
}

// Handle engine tick.
void client_tick(void)
{
}

// Handle post-frame event.
void client_tick_end(void)
{
}

// Initialize the game's shaders for rendering.
void client_initialize_shaders(void)
{
	renderer_shader_t vertex_shader, *client_vertex_shader;
	renderer_shader_t fragment_shader, *client_fragment_shader;
	renderer_program_t program, *client_program;
	renderer_shader_schema_t *client_schema;
	const renderer_shader_utilities_t *shaders;

	// Load the perspective shader.
	shaders = client.game_manager.get_shader_utilities();
	client_program = &client.program;
	shaders->create_program(&client.program);

	// Create vertex and fragment shaders.
	client_vertex_shader = &client.vertex_shader;
	shaders->create_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, client_vertex_shader);
	client_fragment_shader = &client.fragment_shader;
	shaders->create_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, client_fragment_shader);

	// Link both shaders.
	program = *client_program;
	vertex_shader = *client_vertex_shader;
	fragment_shader = *client_fragment_shader;
	shaders->link_shader(vertex_shader, program);
	shaders->link_shader(fragment_shader, program);
	shaders->compile_program(program);

	// Get the renderer schema from the program.
	client_schema = &client.schema;
	shaders->create_shader_schema(program, mesh_attributes, NUM_PERSPECTIVE_SHADER_ATTRIBUTES, client_schema);
}
