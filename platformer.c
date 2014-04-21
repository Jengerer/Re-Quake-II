#include "platformer.h"
#include "player_move.h"
#include <stdio.h>
#include <stdlib.h>

#define PLATFORMER_NAME "Platformer Test"

// Global platformer context.
platformer_context_t platformer;

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
const char* get_platformer_name()
{
	return PLATFORMER_NAME;
}

/*
 * Initialize platformer game.
 * Returns 1 on success, 0 otherwise.
 * Context is filled out as soon as there's something to clean.
 */
int initialize_platformer()
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
	vector3d_set(&mesh->vertices[0].position, -1.0f, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[1].position, 0.0, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[2].position, 0.0f, 0.0f, 0.0f);
	vector3d_set(&mesh->vertices[3].position, -1.0f, 0.0f, 0.0f);
	calculate_polygon_plane(polygon);
	return 1;
}

/*
 * Destroy the platformer context.
 */
void destroy_platformer()
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

	// Create models for the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		if (!renderer->create_indexed_mesh_model(&polygon->indexed_mesh, &polygon->model)) {
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
		renderer->destroy_model(polygon->model);
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

	// Render the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->render_model(polygon->model);
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

