#include "player_move.h"
#include <stdio.h>

#define PLAYER_SPEED 0.05f
#define TURN_RATE 1.0f

/*
 * Handle keyboard input for the arpg.
 */
void handle_player_move(keyboard_manager_t *keyboard, player_move_t *out)
{
	key_state_t left, right, forward, back;
	key_state_t look_left, look_right, look_up, look_down;
	vector3d_t move_direction;
	vector3d_t turn_angles;

	// Simple player movement.
	left = get_key_state(keyboard, ENGINE_KEY_A);
	right = get_key_state(keyboard, ENGINE_KEY_D);
	forward = get_key_state(keyboard, ENGINE_KEY_W);
	back = get_key_state(keyboard, ENGINE_KEY_S);
	look_left = get_key_state(keyboard, ENGINE_KEY_J);
	look_right = get_key_state(keyboard, ENGINE_KEY_L);
	look_up = get_key_state(keyboard, ENGINE_KEY_I);
	look_down = get_key_state(keyboard, ENGINE_KEY_K);

	// Set up movement vector.
	vector3d_clear(&move_direction);
	if ((left & FLAG_KEY_DOWN) != 0) {
		move_direction.x += -PLAYER_SPEED;
	}
	if ((right & FLAG_KEY_DOWN) != 0) {
		move_direction.x += PLAYER_SPEED;
	}
	if ((forward & FLAG_KEY_DOWN) != 0) {
		move_direction.z += PLAYER_SPEED;
	}
	if ((back & FLAG_KEY_DOWN) != 0) {
		move_direction.z += -PLAYER_SPEED;
	}
	vector3d_copy(&move_direction, &out->move_direction);

	// Handle look angles.
	vector3d_clear(&turn_angles);
	if ((look_left & FLAG_KEY_DOWN) != 0) {
		turn_angles.y += -TURN_RATE;
	}
	if ((look_right & FLAG_KEY_DOWN) != 0) {
		turn_angles.y += TURN_RATE;
	}
	if ((look_up & FLAG_KEY_DOWN) != 0) {
		turn_angles.x += -TURN_RATE;
	}
	if ((look_down & FLAG_KEY_DOWN) != 0) {
		turn_angles.x += TURN_RATE;
	}
	vector3d_copy(&turn_angles, &out->turn_angles);
}
