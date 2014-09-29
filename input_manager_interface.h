#define _INPUT_MANAGER_INTERFACE_H_
#define _INPUT_MANAGER_INTERFACE_H_

#include "keyboard_common.h"

// Handle keyboard key event.
typedef void (*handle_keyboard_event_t)(key_code_t key, key_state_t state);

// Interface for receiving input messages.
typedef struct input_manager_interface
{
	handle_keyboard_event_t handle_keyboard_event;
} input_manager_interface_t;

#endif // _INPUT_MANAGER_INTERFACE_H_