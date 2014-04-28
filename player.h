#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "aabb_entity.h"
#include "player_move.h"
#include "vector3d.h"

// Player state object.
typedef struct player
{
	// Player entity.
	aabb_entity_t entity;

	// Player movement cparameters.
	player_move_t move;
} player_t;

// Player initialization.
void initialize_player(player_t *player);

#endif // _PLAYER_H_