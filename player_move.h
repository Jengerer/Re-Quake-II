#ifndef _PLAYER_MOVE_H_
#define _PLAYER_MOVE_H_

#include "vector3d.h"
#include "keyboard_manager.h"

// Object for returning movement parameters.
typedef struct player_move
{
	vector3d_t move_direction;
} player_move_t;

// Input handling callbacks.
void handle_player_move(keyboard_manager_t *keyboard, player_move_t *out);

#endif // _PLAYER_MOVE_H_