#include "game_manager_export.h"
#include "game_manager.h"

// Populate engine listener with game manager callbacks.
void game_manager_populate_engine_listener(engine_listener_t *listener)
{
	listener->on_null = &game_manager_null;
	listener->on_initialize = &game_manager_initialize;
	listener->on_shutdown = &game_manager_shutdown;
	listener->on_tick = &game_manager_tick;
}

// Populate utilities interface for game module.
void game_manager_populate_utilities(game_manager_utilities_t *utilities)
{
	utilities->get_time = &game_manager_get_time;
	utilities->get_renderer = &game_manager_get_renderer;
	utilities->get_shader_utilities = &game_manager_get_shader_utilities;
	utilities->present_frame = &game_manager_present_frame;
}

// Get location of game manager's engine callbacks.
engine_utilities_t *DLL_EXPORT game_manager_get_engine_utilities_destination(void)
{
	return &game_manager.engine;
}

// Get location of game manager's client listener interface.
game_manager_listener_t *game_manager_get_client_listener_destination(void)
{
	return &game_manager.client;
}

// Get location of game manager's server listener interface.
game_manager_listener_t *game_manager_get_server_listener_destination(void)
{
	return NULL;
}
