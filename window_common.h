#ifndef _WINDOW_COMMON_H_
#define _WINDOW_COMMON_H_

#include "input_listener.h"
#include "keyboard_common.h"

// Engine window update flags.
#define WINDOW_FULLSCREEN (1 << 0)
#define WINDOW_BORDERLESS (1 << 1)

// Enumeration for handling window events.
typedef enum window_event_result
{
	WINDOW_EVENT_OK,
	WINDOW_EVENT_QUIT,
	WINDOW_EVENT_ERROR
} window_event_result_t;

// Structure for representing a generic window.
typedef struct window
{
	// Window parameters.
	int width;
	int height;
	int flags;

	// Input event handler to pass messages two.
	input_listener_t input_listener;
} window_t;

// Window structure initialization.
void window_null(window_t *window);

// Window event propagation.
window_event_result_t window_handle_key_press(
	const window_t *window,
	key_code_t key);
window_event_result_t window_handle_key_release(
	const window_t *window,
	key_code_t key);

#endif // _WINDOW_COMMON_H_