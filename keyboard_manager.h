#ifndef _KEYBOARD_MANAGER_H_
#define _KEYBOARD_MANAGER_H_

#include "keyboard_common.h"

// Single key state object.
typedef struct keyboard_key
{
	key_state_t state;
} keyboard_key_t;

// Keyboard key managing object.
typedef struct keyboard_manager
{
	key_state_t states[ENGINE_KEY_MAX];
} keyboard_manager_t;

// Key and manager initialization.
void initialize_keyboard_manager(void);

// Keyboard management functions.
void refresh_keyboard_state(void);
key_state_t get_key_state(key_code_t key_code);
void update_key_state(key_code_t key_code, key_state_t new_state);

// Singleton keyboard retrieval.
const keyboard_manager_t *get_keyboard(void);

#endif // _KEYBOARD_MANAGER_H_