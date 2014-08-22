#include "licht_game.h"
#include "player_move.h"
#include "math_common.h"
#include "object_collision_2d.h"
#include "memory_manager.h"
#include <stdio.h>

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
int initialize_shaders(const renderer_t *renderer);
int add_renderable_object(const object_t *object);
int load_renderable_object_resources(const renderer_t *renderer, renderer_shader_schema_t schema);
void free_renderable_object_resources(const renderer_t *renderer);

/*
 * Null licht context for safe destruction.
 */
void licht_null_context(licht_context_t *context)
{
	player_null(&context->player);
	world_null(&context->world);

	// Null renderables list.
	context->renderable_head = NULL;

	// Null rendering parameters.
	renderer_null_shader(&context->vertex_shader);
	renderer_null_shader(&context->fragment_shader);
	renderer_null_program(&context->program);
	renderer_null_shader_schema(&context->schema);
	renderer_null_uniform(&context->object);
	renderer_null_uniform(&context->view);
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
	dynamic_object_t *player_object;

	// Initialize the world.
	world = &licht.world;
	if (!world_initialize(world)) {
		return 0;
	}

	// Create object for player.
	player = &licht.player;
	player_object = world_create_dynamic_object(world);
	if (player_object == NULL) {
		return 0;
	}
	if (!player_initialize(player, player_object)) {
		return 0;
	}
	if (!add_renderable_object(&player_object->object)) {
		return 0;
	}
	player_object->object.origin.z = 100.0f;

	return 1;
}

/*
 * Destroy the licht context.
 */
void licht_destroy(void)
{
	// Destroy the world.
	world_destroy(&licht.world);
}

/*
 * Load all base resources required by licht.
 */
int licht_load_resources(const renderer_t *renderer)
{
	matrix4x4_t perspective_matrix;

	// Initialize shaders.
	if (!initialize_shaders(renderer)) {
		return 0;
	}

	// Set up the program for the projection matrix.
	renderer->set_program(licht.program);

	// Get the location to the transform and projection matrix.
	if (!renderer->get_uniform(licht.program, "object", &licht.object)) {
		return 0;
	}
	/*if (!renderer->get_uniform(licht.program, "view", &licht.view)) {
		return 0;
	}*/
	if (!renderer->get_uniform(licht.program, "projection", &licht.projection)) {
		return 0;
	}

	// Create renderable models.
	load_renderable_object_resources(renderer, licht.schema);

	// Generate projection matrix.
	matrix4x4_perspective(4.0f / 3.0f, 90.0f, NEAR_DISTANCE, FAR_DISTANCE, &perspective_matrix);
	renderer->set_uniform_matrix4x4(licht.projection, &perspective_matrix);

	// Unset program.
	renderer->unset_program();
	return 1;
}

/*
 * Free base resources for licht.
 */
void licht_free_resources(const renderer_t *renderer)
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
int licht_render(const renderer_t *renderer)
{
	renderable_object_t *renderable;

	// Clear the scene.
	//renderer->clear_scene();

	// Set up the program.
	renderer->set_program(licht.program);

	// Draw the renderable objects.
	renderable = licht.renderable_head;
	while (renderable != NULL) {
		renderable_object_render(renderable, renderer, licht.object, licht.schema);
		renderable = renderable->next;
	}
	
	return 1;
}

/*
 * Handle keyboard input for platjformer.
 */
void licht_handle_keyboard(const keyboard_manager_t *keyboard)
{
	key_state_t key;
	dynamic_object_t *dynamic;
	vector3d_t *velocity;

	// Move player right.
	dynamic = licht.player.dynamic;
	velocity = &dynamic->velocity;
	vector3d_clear(velocity);
	key = get_key_state(keyboard, ENGINE_KEY_D);
	if ((key & FLAG_KEY_DOWN) == FLAG_KEY_DOWN) {
		velocity->x = 5.0f;
	}

	// Move player left.
	key = get_key_state(keyboard, ENGINE_KEY_A);
	if ((key & FLAG_KEY_DOWN) == FLAG_KEY_DOWN) {
		velocity->x = -5.0f;
	}

	// Move player up.
	key = get_key_state(keyboard, ENGINE_KEY_W);
	if ((key & FLAG_KEY_DOWN) == FLAG_KEY_DOWN) {
		velocity->y = 5.0f;
	}

	// Move player down.
	key = get_key_state(keyboard, ENGINE_KEY_S);
	if ((key & FLAG_KEY_DOWN) == FLAG_KEY_DOWN) {
		velocity->y = -5.0f;
	}
}

/* Add an object to the rendering list. */
int add_renderable_object(const object_t *object)
{
	renderable_object_t *renderable;

	// Create renderable object.
	renderable = memory_allocate(sizeof(renderable_object_t));
	if (renderable == NULL) {
		return 0;
	}
	renderable_object_null(renderable);

	// Add to list to be destroyed in case of failure.
	if (licht.renderable_head != NULL) {
		licht.renderable_head->prev = renderable;
	}
	renderable->next = licht.renderable_head;
	licht.renderable_head = renderable;

	// Initialize renderable.
	if (!renderable_object_initialize(renderable, object)) {
		return 0;
	}

	return 1;
}

/* Load all models for the renderables. */
int load_renderable_object_resources(const renderer_t *renderer, renderer_shader_schema_t schema)
{
	renderable_object_t *renderable;

	// Go through each renderable. 
	renderable = licht.renderable_head;
	while (renderable != NULL) {
		if (!renderable_object_load_resources(renderable, renderer, schema)) {
			return 0;
		}
		renderable = renderable->next;
	}

	return 1;
}

/* Destroy all models for the renderables. */
void free_renderable_object_resources(const renderer_t *renderer)
{
	renderable_object_t *renderable;

	// Go through each renderable.
	renderable = licht.renderable_head;
	while (renderable != NULL) {
		renderable_object_free_resources(renderable, renderer);
		renderable = renderable->next;
	}
}

/*
 * Initialize the game's shaders for rendering.
 */
int initialize_shaders(const renderer_t *renderer)
{
	renderer_shader_t vertex_shader, *licht_vertex_shader;
	renderer_shader_t fragment_shader, *licht_fragment_shader;
	renderer_program_t program, *licht_program;
	renderer_shader_schema_t *licht_schema;

	// Load the perspective shader.
	licht_program = &licht.program;
	if (!renderer->create_program(licht_program)) {
		return 0;
	}

	// Create vertex and fragment shaders.
	licht_vertex_shader = &licht.vertex_shader;
	if (!renderer->create_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, licht_vertex_shader)) {
		return 0;
	}
	licht_fragment_shader = &licht.fragment_shader;
	if (!renderer->create_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, licht_fragment_shader)) {
		return 0;
	}

	// Link both shaders.
	program = *licht_program;
	vertex_shader = *licht_vertex_shader;
	fragment_shader = *licht_fragment_shader;
	renderer->link_shader(vertex_shader, program);
	renderer->link_shader(fragment_shader, program);
	if (!renderer->compile_program(program)) {
		return 0;
	}

	// Get the renderer schema from the program.
	licht_schema = &licht.schema;
	if (!renderer->create_shader_schema(program, mesh_attributes, NUM_PERSPECTIVE_SHADER_ATTRIBUTES, licht_schema)) {
		return 0;
	}
	
	return 1;
}
