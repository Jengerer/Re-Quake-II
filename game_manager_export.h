#ifndef _GAME_MANAGER_EXPORT_H_
#define _GAME_MANAGER_EXPORT_H_

#include "shared_defines.h"
#include "engine_listener.h"
#include "engine_utilities.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"

// TODO: rename all of the "get interface" to make it clear someone's filling it.

// Fill in the interface for engine to communicate with game manager.
void DLL_EXPORT game_manager_populate_engine_listener(engine_listener_t *listener);

// Fill in the interace for games to use game manager utilites.
void DLL_EXPORT game_manager_populate_utilities(game_manager_utilities_t *utilities);

// Get the structure for the engine to fill with utilities.
engine_utilities_t *DLL_EXPORT game_manager_get_engine_utilities_destination(void);

// Get the listener structure that the client needs to fill out.
game_manager_listener_t *DLL_EXPORT game_manager_get_client_listener_destination(void);

// Get the listener structure that the server needs to fill out.
game_manager_listener_t *DLL_EXPORT game_manager_get_server_listener_destination(void);

#endif // _GAME_MANAGER_EXPORT_H_