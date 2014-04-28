#include "player_move.h"
#include <stdio.h>

#define PLAYER_SPEED 1.0f

/*
 * Handle keyboard input for the platformer.
 */
void handle_player_move(keyboard_manager_t *keyboard, player_move_t *out)
{
	key_state_t left;
	key_state_t right;
	key_state_t forward;
	key_state_t back;
	vector3d_t move_direction;

	// Simple player movement.
	left = get_key_state(keyboard, ENGINE_KEY_A);
	right = get_key_state(keyboard, ENGINE_KEY_D);
	forward = get_key_state(keyboard, ENGINE_KEY_W);
	back = get_key_state(keyboard, ENGINE_KEY_S);

	// Set up movement vector.
	vector3d_clear(&move_direction);
	if ((left & FLAG_KEY_DOWN) != 0) {
		move_direction.x += -PLAYER_SPEED;
	}
	if ((right & FLAG_KEY_DOWN) != 0) {
		move_direction.x += PLAYER_SPEED;
	}
	if ((forward & FLAG_KEY_DOWN) != 0) {
		move_direction.z += -PLAYER_SPEED;
	}
	if ((back & FLAG_KEY_DOWN) != 0) {
		move_direction.z += PLAYER_SPEED;
	}
	vector3d_copy(&move_direction, &out->move_direction);
}
