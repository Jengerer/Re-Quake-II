#ifndef _GAME_MANAGER_LISTENER_H_
#define _GAME_MANAGER_LISTENER_H_

// Function types for game interface.
typedef void (*game_manager_null_t)(void);
typedef void (*game_manager_initialize_t)(void);
typedef void (*game_manager_shutdown_t)(void);
typedef void (*game_manager_tick_begin_t)(void);
typedef void (*game_manager_tick_t)(void);
typedef void (*game_manager_tick_end_t)(void);

// Structure for representing an interface to a game implementation.
typedef struct game_manager_listener
{
	// Game initialization/destruction interface functions.
	game_manager_null_t on_null;
	game_manager_initialize_t on_initialize;
	game_manager_shutdown_t on_shutdown;

	// Game running events (in order of execution).
	game_manager_tick_begin_t on_tick_begin;
	game_manager_tick_t on_tick;
	game_manager_tick_end_t on_tick_end;
} game_manager_listener_t;

#endif // _GAME_MANAGER_LISTENER_H_
