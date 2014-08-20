#include "player.h"
#include "memory_manager.h"

/* Null player object for safe destruction. */
void player_null(player_t *player)
{
	player->object = NULL;
}

/*
 * Initialize player entity.
 */
void player_initialize(player_t *player, world_object_t *object)
{
	player->object = object;
}

/* Destroy player entity. */
void player_destroy(player_t *player)
{
	(void)player;
}
