#include "platformer.h"
#include <stdio.h>
#include <stdlib.h>

#define PLATFORMER_NAME "Platformer Test"

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
	game->get_name = &get_platformer_name;
	game->initialize = &initialize_platformer;
	game->destroy = &destroy_platformer;
	game->load_resources = &load_platformer_resources;
	game->free_resources = &free_platformer_resources;
	game->render = &render_platformer;
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
int initialize_platformer(game_context_t **out)
{
	aabb_t *player;
	map_t *map;
	polygon_t *polygon;
	mesh_t *mesh;
	indexed_mesh_t *indexed_mesh;
	platformer_context_t *context = (platformer_context_t*)malloc(sizeof(platformer_context_t));
	if (context == NULL) {
		return 0;
	}
	null_platformer_context(context);
	*out = (game_context_t*)context;

	// Create player.
	player = &context->player;
	vector_set(&player->mins, -0.25f, -0.25f, -0.25f);
	vector_set(&player->maxs, 0.25f, 0.25f, 0.25f);
	vector_set(&player->position, 0.0f, 0.0f, 0.0f);

	// Initialize the map.
	map = &context->map;
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
	vector_set(&mesh->vertices[0], -1.0f, 1.0f, 0.0f);
	vector_set(&mesh->vertices[1], 0.0, 1.0f, 0.0f);
	vector_set(&mesh->vertices[2], 0.0f, 0.0f, 0.0f);
	vector_set(&mesh->vertices[3], -1.0f, 0.0f, 0.0f);
	calculate_polygon_plane(polygon);
	return 1;
}

/*
 * Destroy the platformer context.
 */
void destroy_platformer(game_context_t *context)
{
	platformer_context_t *platformer;
	
	// Check if we even started initializing.
	if (context == NULL) {
		return;
	}
	platformer = (platformer_context_t*)context;

	// Destroy the map.
	destroy_map(&platformer->map);
}

/*
 * Load all base resources required by platformer.
 */
int load_platformer_resources(game_context_t *context, renderer_t *renderer)
{
	int i;
	platformer_context_t *platformer;
	map_t *map;
	polygon_t *polygon;

	platformer = (platformer_context_t*)context;
	map = &platformer->map;

	// Create models for the map polygons.
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		if (!renderer->create_indexed_mesh_model(renderer->context, &polygon->indexed_mesh, &polygon->model)) {
			return 0;
		}
	}
	return 1;
}

/*
 * Free base resources for platformer.
 */
void free_platformer_resources(game_context_t *context, renderer_t *renderer)
{
	int i;
	platformer_context_t *platformer;
	map_t *map;
	polygon_t *polygon;

	platformer = (platformer_context_t*)context;
	map = &platformer->map;

	// Create models for the map polygons.
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->destroy_model(&renderer->context, polygon->model);
	}
}

/*
 * Platformer rendering function.
 */
int render_platformer(game_context_t *context, renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;
	platformer_context_t *platformer = (platformer_context_t*)context;

	// Render the map polygons.
	map = &platformer->map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->render_model(&renderer->context, polygon->model);
	}
	return 1;
}
