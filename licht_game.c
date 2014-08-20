#include "licht_game.h"
#include "player_move.h"
#include "math_common.h"
#include <stdio.h>
#include <stdlib.h>

// Game parameters.
#define PLATFORMER_NAME "ARPG Test"

// Rendering parameters.
#define NUM_PERSPECTIVE_SHADER_ATTRIBUTES 2
#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"

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
	null_map(&context->map);
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
	return PLATFORMER_NAME;
}

/*
 * Initialize licht game.
 * Returns 1 on success, 0 otherwise.
 * Context is filled out as soon as there's something to clean.
 */
int licht_initialize(void)
{
	player_t *player;
	map_t *map;
	polygon_t *polygon;
	mesh_t *mesh;
	indexed_mesh_t *indexed_mesh;

	// Create player.
	player = &licht.player;

	// Initialize the map.
	map = &licht.map;
	if (!initialize_map(map, 1)) {
		printf("Failed to initialize map.\n");
		return 0;
	}
	polygon = &map->polygons[0];
	indexed_mesh = &polygon->indexed_mesh;
	mesh = &indexed_mesh->mesh;
	if (!polygon_initialize(polygon, 4)) {
		printf("Failed to initialize polygon.\n");
		return 0;
	}
	vector3d_set(&mesh->vertices[0].position, -1.0f, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[1].position, 0.0, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[2].position, 0.0f, 0.0f, 0.0f);
	vector3d_set(&mesh->vertices[3].position, -1.0f, 0.0f, 0.0f);
	vector2d_set(&mesh->vertices[0].texture, 0.0f, 0.0f);
	vector2d_set(&mesh->vertices[1].texture, 1.0f, 0.0f);
	vector2d_set(&mesh->vertices[2].texture, 1.0f, 1.0f);
	vector2d_set(&mesh->vertices[3].texture, 0.0f, 1.0f);
	polygon_calculate_plane(polygon);
	return 1;
}

/*
 * Destroy the licht context.
 */
void licht_destroy(void)
{
	// Destroy the map.
	destroy_map(&licht.map);
}

/*
 * Load all base resources required by licht.
 */
int licht_load_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;
	indexed_mesh_t *indexed_mesh;
	mesh_t *mesh;
	matrix4x4_t perspective_matrix;
	vector3d_t test3;
	vector4d_t test, test2;

	// Initialize shaders.
	if (!initialize_shaders(renderer)) {
		return 0;
	}

	// Set up shader for models.
	renderer->set_program(licht.program);

	// Create models for the map polygons.
	map = &licht.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		indexed_mesh = &polygon->indexed_mesh;
		mesh = &indexed_mesh->mesh;
		if (!renderer->create_indexed_model(mesh->vertices,
			mesh->num_vertices,
			indexed_mesh->indices,
			indexed_mesh->num_indices,
			licht.schema,
			&polygon->model))
		{
			return 0;
		}
	}

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
	matrix4x4_perspective(4.0f / 3.0f, 90.0f, 1.0f, 1000.0f, &perspective_matrix);
	matrix4x4_transform(&perspective_matrix, &test, &test2);
	vector4d_to_vector3d(&test2, &test3);
	renderer->set_uniform_matrix4x4(licht.projection, &perspective_matrix);
	return 1;
}

/*
 * Free base resources for licht.
 */
void licht_free_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;

	// Destroy models for the map polygons.
	map = &licht.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->destroy_model(&polygon->model);
	}

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
