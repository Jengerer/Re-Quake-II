#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "shared_defines.h"
#include "engine_listener.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"

// Fill in the interface for engine to communicate with game manager.
void DLL_EXPORT populate_engine_listener(engine_listener_t *listener);

// Fill in the interace for games to use game manager utilites.
void DLL_EXPORT populate_game_manager_utilities(game_manager_utilities_t *utilities);

// Get the listener structure that the client needs to fill out.
game_manager_listener_t *DLL_EXPORT get_client_listener_interface(void);

// Get the listener structure that the server needs to fill out.
game_manager_listener_t *DLL_EXPORT get_server_listener_interface(void);

#endif // _GAME_MANAGER_H_