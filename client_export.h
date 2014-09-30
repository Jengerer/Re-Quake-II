#ifndef _GAME_EXPORT_H_
#define _GAMET_EXPORT_H_

#include "shared_defines.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"

// TODO: Combine server and client export once they're actually separate modules.

// Request the game module to fill out the game manager listener structure.
void DLL_EXPORT client_populate_game_manager_listener(game_manager_listener_t *listener);

// Get the structure for the game module to fill out for utilities.
game_manager_utilities_t *DLL_EXPORT client_get_game_manager_utilities_destination(void);

#endif // _CLIENT_EXPORT_H_