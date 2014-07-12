#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"
#include "player_move.h"
#include "vector3d.h"

// Player state object.
typedef struct player
{
	// Player entity.
	entity_t entity;

	// Player movement parameters.
	player_move_t move;
} player_t;

// Player initialization.
void initialize_player(player_t *player);

#endif // _PLAYER_H_