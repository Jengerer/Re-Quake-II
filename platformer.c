#include "platformer.h"
#include "player_move.h"
#include <stdio.h>
#include <stdlib.h>

// Game parameters.
#define PLATFORMER_NAME "Platformer Test"

// Rendering parameters.
#define NUM_PERSPECTIVE_SHADER_ATTRIBUTES 2
#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"

// Global platformer context.
static platformer_context_t platformer;

// Renderer shader schemas.
const renderer_shader_attribute_t mesh_attributes[NUM_PERSPECTIVE_SHADER_ATTRIBUTES] = {
	{"in_vertex", ATTRIBUTE_VERTEX_3D},
	{"in_texture", ATTRIBUTE_TEXTURE_COORDINATE}
};

// Initialize schema.
const renderer_shader_schema_t mesh_schema = {
	mesh_attributes, NUM_PERSPECTIVE_SHADER_ATTRIBUTES
};

// Private functions.
int initialize_shaders(renderer_t *renderer);

/*
 * Null platformer context for safe destruction.
 */
void null_platformer_context(platformer_context_t *context)
{
	null_map(&context->map);
}

/*
 * Fill out game interface with platformer functions.
 */
void initialize_platformer_interface(game_t *game)
{
	// Null context here.
	null_platformer_context(&platformer);

	// Initialize interface functions.
	game->get_name = &get_platformer_name;
	game->initialize = &initialize_platformer;
	game->destroy = &destroy_platformer;
	game->load_resources = &load_platformer_resources;
	game->free_resources = &free_platformer_resources;
	game->render = &render_platformer;
	game->handle_keyboard = &handle_platformer_keyboard;
}

/*
 * Return name of the platformer game.
 */
const char* get_platformer_name(void)
{
	return PLATFORMER_NAME;
}

/*
 * Initialize platformer game.
 * Returns 1 on success, 0 otherwise.
 * Context is filled out as soon as there's something to clean.
 */
int initialize_platformer(void)
{
	player_t *player;
	map_t *map;
	polygon_t *polygon;
	mesh_t *mesh;
	indexed_mesh_t *indexed_mesh;

	// Create player.
	player = &platformer.player;
	initialize_player(player);

	// Initialize the map.
	map = &platformer.map;
	if (!initialize_map(map, 1)) {
		printf("Failed to initialize map.\n");
		return 0;
	}
	polygon = &map->polygons[0];
	indexed_mesh = &polygon->indexed_mesh;
	mesh = &indexed_mesh->mesh;
	if (!initialize_polygon(polygon, 4)) {
		printf("Failed to initialize polygon.\n");
		return 0;
	}
	vector3d_set(&mesh->vertices[0].position, -1.0f, 1.0f, 4.0f);
	vector3d_set(&mesh->vertices[1].position, 0.0, 1.0f, 4.0f);
	vector3d_set(&mesh->vertices[2].position, 0.0f, 0.0f, 4.0f);
	vector3d_set(&mesh->vertices[3].position, -1.0f, 0.0f, 4.0f);
	vector2d_set(&mesh->vertices[0].texture, 0.0f, 0.0f);
	vector2d_set(&mesh->vertices[1].texture, 1.0f, 0.0f);
	vector2d_set(&mesh->vertices[2].texture, 1.0f, 1.0f);
	vector2d_set(&mesh->vertices[3].texture, 0.0f, 1.0f);
	calculate_polygon_plane(polygon);
	return 1;
}

/*
 * Destroy the platformer context.
 */
void destroy_platformer(void)
{
	// Destroy the map.
	destroy_map(&platformer.map);
}

/*
 * Load all base resources required by platformer.
 */
int load_platformer_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;
	indexed_mesh_t *indexed_mesh;
	mesh_t *mesh;

	// Initialize shaders.
	if (!initialize_shaders(renderer)) {
		return 0;
	}

	// Set up shader for models.
	renderer->set_shader_program(platformer.program);

	// Create models for the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		indexed_mesh = &polygon->indexed_mesh;
		mesh = &indexed_mesh->mesh;
		if (!renderer->create_indexed_model(mesh->vertices,
			mesh->num_vertices,
			indexed_mesh->indices,
			indexed_mesh->num_indices,
			&mesh_schema,
			&polygon->model))
		{
			return 0;
		}
	}
	return 1;
}

/*
 * Free base resources for platformer.
 */
void free_platformer_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;

	// Destroy models for the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->destroy_model(&polygon->model);
	}
}

/*
 * Platformer rendering function.
 */
int render_platformer(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;

	// Clear the scene.
	renderer->clear_scene();
	renderer->set_shader_program(platformer.program);

	// Render the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->render_model(&polygon->model);
	}
	return 1;
}

/*
 * Handle keyboard input for platjformer.
 */
void handle_platformer_keyboard(keyboard_manager_t *keyboard)
{
	player_t *player;
	player_move_t *move;
	entity_t *player_entity;
	vector3d_t *position;
	vector3d_t *velocity;

	player = &platformer.player;
	move = &player->move;
	player_entity = &player->entity.base;
	position = &player_entity->origin;
	velocity = &move->move_direction;
	
	// Update player movement.
	handle_player_move(keyboard, move);

	// Move the player by the command.
	vector3d_add(position, velocity, position);
	if (vector3d_magnitude(velocity) != 0.0f) {
		printf("VEL: (%f, %f, %f)\n", velocity->x, velocity->y, velocity->z);
		printf("POS: (%f, %f, %f)\n", position->x, position->y, position->z);
	}
}

/*
 * Initialize the platformer's shaders for rendering.
 */
int initialize_shaders(renderer_t *renderer)
{
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_shader_program_t *shader_program;

	// Load the perspective shader.
	shader_program = &platformer.program;
	if (!renderer->create_shader_program(shader_program, &mesh_schema)) {
		return 0;
	}

	// Create vertex and fragment shaders.
	if (!renderer->create_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, &vertex_shader)) {
		return 0;
	}
	platformer.vertex_shader = vertex_shader;
	if (!renderer->create_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, &fragment_shader)) {
		return 0;
	}
	platformer.fragment_shader = fragment_shader;

	// Link both shaders.
	renderer->link_shader(vertex_shader, shader_program);
	renderer->link_shader(fragment_shader, shader_program);
	if (!renderer->compile_shader_program(shader_program)) {
		return 0;
	}
	
	return 1;
}
