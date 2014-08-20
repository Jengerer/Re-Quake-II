#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "world_object.h"
#include "player_move.h"
#include "vector3d.h"

// Player state object.
typedef struct player
{
	// Player physics entity.
	world_object_t *object;

	// Player movement parameters.
	player_move_t move;
} player_t;

// Player initialization.
void player_null(player_t *player);
void player_initialize(player_t *player, world_object_t *object);
void player_destroy(player_t *player);

#endif // _PLAYER_H_