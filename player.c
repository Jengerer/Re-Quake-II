#include "player.h"

// Player size constraints.
static const vector3d_t PLAYER_HULL_MIN = {-16.0f, -16.0f, -32.0f};
static const vector3d_t PLAYER_HULL_MAX = {16.0f, 16.0f, 32.0f};

/*
 * Initialize player entity.
 */
void initialize_player(player_t *player)
{
	aabb_t *aabb;
	aabb_entity_t *entity = &player->entity;
	aabb = &entity->aabb;

	// Set up player's size.
	vector3d_copy(&PLAYER_HULL_MIN, &aabb->mins);
	vector3d_copy(&PLAYER_HULL_MAX, &aabb->maxs);
	vector3d_clear(&entity->base.origin);
}
