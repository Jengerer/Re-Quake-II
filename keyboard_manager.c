#include "keyboard_manager.h"

// Singleton instance.
static keyboard_manager_t keyboard;

// Private functions.
void null_keyboard_key_state(key_state_t *key);

/*
 * Initialize keyboard manager object.
 */
void initialize_keyboard_manager(void)
{
	int i;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		null_keyboard_key_state(&keyboard.states[i]);
	}
}

/*
 * Reset the "changed" flag for the keys for a new frame.
 */
void refresh_keyboard_state(void)
{
	int i;
	key_state_t *state;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		state = &keyboard.states[i];
		state->flags.changed = 0;
	}
}

/*
 * Return key state object reference for the engine key code.
 */
key_state_t get_key_state(key_code_t key_code)
{
	return keyboard.states[key_code];
}

/*
 * Update keyboard state in the manner.
 */
void update_key_state(key_code_t key_code, key_state_t new_state)
{
	key_state_t *state = &keyboard.states[key_code];
	*state = new_state;
}

// Retrieve singleton keyboard manager instance.
// Keyboard must have been initialized before any calls to this.
const keyboard_manager_t *get_keyboard(void)
{
	return &keyboard;
}

// Clear keyboard key state object.
void null_keyboard_key_state(key_state_t *state)
{
	state->as_char = 0;
}
