#include "engine_export.h"
#include "engine.h"

// Populate engine interface for application to use.
void engine_populate_interface(engine_interface_t *engine_interface)
{
	engine_interface->initialize = &engine_initialize;
	engine_interface->shutdown = &engine_shutdown;
	engine_interface->run = &engine_run;
}

// Populate engine utilities for game manager to call.
void engine_populate_utilities(engine_utilities_t *utilities)
{
	utilities->create_window = &engine_create_window;
	utilities->update_window = &engine_update_window;
	utilities->swap_buffers = &engine_swap_buffers;
	utilities->get_renderer = &engine_get_renderer;
	utilities->get_shader_utilities = &engine_get_shader_utilities;
}

// Get location of engine listener interface.
engine_listener_t *DLL_EXPORT engine_get_listener_destination(void)
{
	return &engine.listener;
}

// Get location of engine's renderer interface.
renderer_t *DLL_EXPORT engine_get_renderer_destination(void)
{
	return &engine.renderer;
}

// Get location of engine's shader utilities.
renderer_shader_utilities_t *DLL_EXPORT engine_get_shader_utilities_destination(void)
{
	return &engine.shaders;
}
