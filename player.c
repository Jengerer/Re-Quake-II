#include "player.h"

/*
 * Initialize player entity.
 */
void initialize_player(player_t *player)
{
	entity_t *entity;
	vector3d_t *origin;

	// Set player position to zero.
	entity = &player->entity;
	origin = &entity->origin;
	vector3d_clear(origin);
}
