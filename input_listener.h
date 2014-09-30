#ifndef _INPUT_LISTENER_H_
#define _INPUT_LISTENER_H_

#include "keyboard_common.h"

// Input event return values.
typedef enum input_event_result
{
	INPUT_EVENT_OK,
	INPUT_EVENT_QUIT,
	INPUT_EVENT_ERROR
} input_event_result_t;

// Keyboard event handler.
typedef input_event_result_t (*keyboard_event_t)(key_code_t key);

// Interface for an input event handler.
typedef struct input_listener
{
	keyboard_event_t on_key_press;
	keyboard_event_t on_key_release;
} input_listener_t;

#endif // _INPUT_LISTENER_H_