#include "client_export.h"
#include "game_client.h"

// Fill out game manager listener functions with client implementation.
void populate_client_game_manager_listener(game_manager_listener_t *listener)
{
	listener->on_null = &client_null;
	listener->on_initialize = &client_initialize;
	listener->on_shutdown = &client_shutdown;
	listener->on_tick_begin = &client_tick_begin;
	listener->on_tick = &client_tick;
	listener->on_tick_end = &client_tick_end;
}

// Get structure for game manager to fill with utilities.
game_manager_utilities_t *get_client_game_manager_utilities(void)
{
	return &client.game_manager;
}
