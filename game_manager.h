#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "engine_utilities.h"
#include "game_manager_listener.h"

// The game manager is layered between the engine and the game modules.
// It transmits messages between the client and server, manages resources,
// input systems, and relegates messages from the engine.
typedef struct game_manager
{
	engine_utilities_t engine;
	game_manager_listener_t client;
} game_manager_t;

// Engine listener functions.
void game_manager_null(void);
int game_manager_initialize(void);
void game_manager_shutdown(void);
int game_manager_tick(float delta_time);

// Singleton instance of game manager.
extern game_manager_t game_manager;

#endif // _GAME_MANAGER_H_