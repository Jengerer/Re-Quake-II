#include "player.h"

/*
 * Initialize player entity.
 */
void initialize_player(player_t *player)
{
	entity_t *entity;
	vector3d_t *origin;
	vector3d_t *angles;

	// Set player position to zero.
	entity = &player->entity;
	origin = &entity->origin;
	angles = &entity->angles;
	vector3d_clear(origin);
	vector3d_clear(angles);
}