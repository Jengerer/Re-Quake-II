#ifndef _GAME_MANAGER_UTILITIES_H_
#define _GAME_MANAGER_UTILITIES_H_

// Request current time.
typedef float (*get_current_time_t)(void);

// Interface for a game to request resources and perform tasks from the game manager.
typedef struct game_manager_utilities
{
	get_current_time_t get_current_time;
} game_manager_utilities_t;

#endif // _GAME_MANAGER_UTILITIES_H_