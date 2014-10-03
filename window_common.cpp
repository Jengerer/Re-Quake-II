#include "window_common.h"
#include "error_stack.h"

// Private functions.
window_event_result_t translate_input_result(input_event_result_t result);

// Clear window structure.
void window_null(window_t *window)
{
	window->width = 0;
	window->height = 0;
	window->flags = 0;
}

// Propagate a keyboard press event to the listener.
window_event_result_t window_handle_key_press(
	const window_t *window,
	key_code_t key)
{
	input_event_result_t result;
	const input_listener_t *listener = &window->input_listener;
	result = listener->on_key_press(key);
	return translate_input_result(result);	
}

// Propagate a keyboard release event to the listener.
window_event_result_t window_handle_key_release(
	const window_t *window,
	key_code_t key)
{
	input_event_result_t result;
	const input_listener_t *listener = &window->input_listener;
	result = listener->on_key_release(key);
	return translate_input_result(result);	
}

// Translate an input event result to a window event result.
window_event_result_t translate_input_result(input_event_result_t result)
{
	switch (result) {
	case INPUT_EVENT_OK:
		return WINDOW_EVENT_OK;
	case INPUT_EVENT_QUIT:
		return WINDOW_EVENT_QUIT;
	case INPUT_EVENT_ERROR:
		return WINDOW_EVENT_ERROR;
	default:
		error_stack_log("Invalid input event result received.\n");
		return WINDOW_EVENT_ERROR;
	}
}
