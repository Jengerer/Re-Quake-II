#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "player_move.h"
#include "renderer.h"
#include "vector3d.h"

// Player state object.
typedef struct player
{
	// Player physics entity.
	dynamic_object_t *dynamic;

	// Player movement parameters.
	player_move_t move;
} player_t;

// Player initialization.
void player_null(player_t *player);
int player_initialize(player_t *player, dynamic_object_t *dynamic);
void player_destroy(player_t *player);

#endif // _PLAYER_H_