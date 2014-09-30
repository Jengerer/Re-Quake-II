#include "game_manager_export.h"
#include "game_manager.h"

// Populate engine listener with game manager callbacks.
void game_manager_populate_engine_listenr(engine_listener_t *listener)
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
	utilities->
}