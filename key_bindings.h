#ifndef _KEY_BINDINGS_H_
#define _KEY_BINDINGS_H_

#include <SDL.h>

// Enum for storing a key's state.
typedef enum key_state
{
	KEY_CLICKED,  // Going from up to down.
	KEY_DOWN,
	KEY_RELEASED, // Going from down to up.
	KEY_UP,
};

// Struct for managing a single key to command binding.
// TODO: May be better to use a map.
typedef struct key_binding
{
	int key_code;
	int key_state;
	key_binding_t *next;
} key_binding_t;

// Struct for handling key bindings.
typedef struct binding_map
{
	key_binding_t *head;
	int num_bindings;
} binding_map_t;

// Binding map initialization/destruction.
void null_binding_map(binding_map_t *map);
void destroy_binding_map(binding_map_t *map);

// Binding handling.
int add_key_binding(binding_map_t *map, int key_code, int command_id);

#endif // _KEY_BINDINGS_H_