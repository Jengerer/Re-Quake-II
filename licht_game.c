#include "licht_game.h"
#include "player_move.h"
#include "math_common.h"
#include <stdio.h>
#include <stdlib.h>

// Game parameters.
#define LICHT_NAME "Licht"

// Rendering parameters.
#define NUM_PERSPECTIVE_SHADER_ATTRIBUTES 2
#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"
#define NEAR_DISTANCE 1.0f
#define FAR_DISTANCE 1024.0f

// Global game context.
static licht_context_t licht;

// Renderer shader schemas.
const renderer_shader_attribute_t mesh_attributes[NUM_PERSPECTIVE_SHADER_ATTRIBUTES] = {
	{"in_vertex", VARIABLE_VERTEX_3D},
	{"in_texture", VARIABLE_VERTEX_2D}
};

// Private functions.
int initialize_shaders(renderer_t *renderer);

/*
 * Null licht context for safe destruction.
 */
void licht_null_context(licht_context_t *context)
{
	map_null(&context->map);
	world_null(&context->world);
	renderer_null_shader(&context->vertex_shader);
	renderer_null_shader(&context->fragment_shader);
	renderer_null_program(&context->program);
	renderer_null_shader_schema(&context->schema);
	renderer_null_uniform(&context->projection);
}

/*
 * Fill out game interface with licht functions.
 */
void licht_initialize_interface(game_t *game)
{
	// Null context here.
	licht_null_context(&licht);

	// Initialize interface functions.
	game->get_name = &licht_get_name;
	game->initialize = &licht_initialize;
	game->destroy = &licht_destroy;
	game->load_resources = &licht_load_resources;
	game->free_resources = &licht_free_resources;
	game->render = &licht_render;
	game->handle_keyboard = &licht_handle_keyboard;
}

/*
 * Return name of the licht game.
 */
const char* licht_get_name(void)
{
	return LICHT_NAME;
}

/*
 * Initialize licht game.
 * Returns 1 on success, 0 otherwise.
 * Context is filled out as soon as there's something to clean.
 */
int licht_initialize(void)
{
	world_t *world;
	player_t *player;
	object_t *player_object;

	// Initialize the world.
	world = &licht.world;
	if (!world_initialize(world)) {
		return 0;
	}

	// Create object for player.
	player = &licht.player;
	player_object = world_create_object(world);
	if (player_object == NULL) {
		return 0;
	}
	player_initialize(player, player_object);
	return 1;
}

/*
 * Destroy the licht context.
 */
void licht_destroy(void)
{
	// Destroy the map.
	map_destroy(&licht.map);

	// Destroy the world.
	world_destroy(&licht.world);
}

/*
 * Load all base resources required by licht.
 */
int licht_load_resources(renderer_t *renderer)
{
	matrix4x4_t perspective_matrix;

	// Initialize shaders.
	if (!initialize_shaders(renderer)) {
		return 0;
	}

	// Set up shader for models.
	renderer->set_program(licht.program);

	// Get the location to the transform and projection matrix.
	if (!renderer->get_uniform(licht.program, "object", &licht.object)) {
		return 0;
	}
	if (!renderer->get_uniform(licht.program, "view", &licht.view)) {
		return 0;
	}
	if (!renderer->get_uniform(licht.program, "projection", &licht.projection)) {
		return 0;
	}

	// Generate projection matrix.
	matrix4x4_perspective(4.0f / 3.0f, 90.0f, NEAR_DISTANCE, FAR_DISTANCE, &perspective_matrix);
	renderer->set_uniform_matrix4x4(licht.projection, &perspective_matrix);
	return 1;
}

/*
 * Free base resources for licht.
 */
void licht_free_resources(renderer_t *renderer)
{
	// Release uniform variable handles.
	renderer->destroy_uniform(&licht.object);
	renderer->destroy_uniform(&licht.view);
	renderer->destroy_uniform(&licht.projection);

	// Destroy schema.
	renderer->destroy_shader_schema(&licht.schema);

	// Destroy program and shaders.
	renderer->destroy_shader(&licht.vertex_shader, licht.program);
	renderer->destroy_shader(&licht.fragment_shader, licht.program);
	renderer->destroy_program(&licht.program);
}

/*
 * ARPG rendering function.
 */
int licht_render(renderer_t *renderer)
{
	(void)renderer;
	return 1;
}

/*
 * Handle keyboard input for platjformer.
 */
void licht_handle_keyboard(keyboard_manager_t *keyboard)
{
	(void)keyboard;
}

/*
 * Initialize the licht's shaders for rendering.
 */
int initialize_shaders(renderer_t *renderer)
{
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_program_t program;
	renderer_shader_schema_t schema;

	// Load the perspective shader.
	if (!renderer->create_program(&program)) {
		return 0;
	}
	licht.program = program;

	// Create vertex and fragment shaders.
	if (!renderer->create_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, &vertex_shader)) {
		return 0;
	}
	licht.vertex_shader = vertex_shader;
	if (!renderer->create_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, &fragment_shader)) {
		return 0;
	}
	licht.fragment_shader = fragment_shader;

	// Link both shaders.
	renderer->link_shader(vertex_shader, program);
	renderer->link_shader(fragment_shader, program);
	if (!renderer->compile_program(program)) {
		return 0;
	}

	// Get the renderer schema from the program.
	if (!renderer->create_shader_schema(program, mesh_attributes, NUM_PERSPECTIVE_SHADER_ATTRIBUTES, &schema)) {
		return 0;
	}
	licht.schema = schema;
	
	return 1;
}
