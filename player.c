#include "player.h"
#include "memory_manager.h"

// Player world object parameters.
#define PLAYER_WIDTH 32.0f
#define PLAYER_HEIGHT 32.0f

/* Null player object for safe destruction. */
void player_null(player_t *player)
{
	player->dynamic = NULL;
}

/*
 * Initialize player entity.
 */
int player_initialize(player_t *player, dynamic_object_t *dynamic)
{
	object_t *object;
	polygon_t *polygon;

	// Set player object in case initializing fails.
	player->dynamic = dynamic;

	// Create the polygon.
	object = &dynamic->object;
	polygon = &object->polygon;
	if (!polygon_create_rectangle(polygon, PLAYER_WIDTH, PLAYER_HEIGHT)) {
		return 0;
	}
	return 1;
}

/* Destroy player entity. */
void player_destroy(player_t *player)
{
	(void)player;
}
