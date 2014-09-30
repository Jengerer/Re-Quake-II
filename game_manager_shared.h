#ifndef _GAME_MANAGER_SHARED_H_
#define _GAME_MANAGER_SHARED_H_

#include "renderer.h"
#include "renderer_shader_utilities.h"

// Types that the game manager shares with game modules.
typedef enum game_manager_error
{
	GAME_FATAL_ERROR, // Client and server should be shut down.
	GAME_CLIENT_DROP, // Client should be removed from server.
	GAME_SERVER_DROP, // Server should be shut down.
} game_manager_error_t;

#endif // _GAME_MANAGER_SHARED_H_
