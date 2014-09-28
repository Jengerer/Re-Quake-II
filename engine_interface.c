#include "engine_interface.h"
#include "memory_manager.h"

// Instantiate singleton.
engine_interface_t engine_functions = NULL;

// Null engine interface functions.
void engine_interface_null(void)
{
	// Display functions.
	engine_functions.update_window = NULL;

	// Resource functions.
	engine_functions.get_graphics_resources = NULL;
}
