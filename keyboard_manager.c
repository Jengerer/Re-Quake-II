#include "keyboard_manager.h"

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
void initialize_keyboard_manager(keyboard_manager_t *manager)
{
	int i;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		null_keyboard_key_state(&manager->keys[i]);
	}
}

/*
 * Reset the "changed" flag for the keys for a new frame.
 */
void refresh_keyboard_state(keyboard_manager_t *manager)
{
	int i;
	keyboard_key_t *key;
	for (i = 0; i < ENGINE_KEY_MAX; ++i) {
		key = &manager->keys[i];
		key->state &= ~(FLAG_KEY_DOWN);
	}
}

/*
 * Return key state object reference for the engine key code.
 */
key_state_t get_key_state(const keyboard_manager_t *manager, key_code_t key_code)
{
	const keyboard_key_t *key = &manager->keys[key_code];
	return key->state;
}

/*
 * Update keyboard state in the manner.
 */
void update_key_state(keyboard_manager_t *manager, key_code_t key_code, key_state_t new_state)
{
	keyboard_key_t *key = &manager->keys[key_code];
	key->state = new_state;
}
