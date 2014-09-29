#include "keyboard_manager.h"

// Singleton instance.
static keyboard_manager_t keyboard;

// Private functions.
void null_keyboard_key_state(keyboard_key_t *key);

/*
 * Clear keyboard key state object.
 */
void null_keyboard_key_state(keyboard_key_t *key)
{
	key->state = 0;
}

/*
 * Initialize keyboard manager object.
 */
void initialize_keyboard_manager(void)
{
	int i;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		null_keyboard_key_state(&keyboard.keys[i]);
	}
}

/*
 * Reset the "changed" flag for the keys for a new frame.
 */
void refresh_keyboard_state(void)
{
	int i;
	keyboard_key_t *key;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		key = &keyboard.keys[i];
		key->state &= ~(FLAG_KEY_DOWN);
	}
}

/*
 * Return key state object reference for the engine key code.
 */
key_state_t get_key_state(key_code_t key_code)
{
	const keyboard_key_t *key = &keyboard.keys[key_code];
	return key->state;
}

/*
 * Update keyboard state in the manner.
 */
void update_key_state(keyboard_manager_t *manager, key_code_t key_code, key_state_t new_state)
{
	keyboard_key_t *key = &keyboard.keys[key_code];
	key->state = new_state;
}

// Retrieve singleton keyboard manager instance.
// Keyboard must have been initialized before any calls to this.
const keyboard_manager_t *get_keyboard(void)
{
	return &keyboard;
}
