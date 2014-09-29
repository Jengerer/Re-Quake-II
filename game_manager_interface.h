#ifndef _GAME_MANAGER_INTERFACE_H_
#define _GAME_MANAGER_INTERFACE_H_

#include "keyboard_manager.h"

// Initialization and clean-up events.
typedef int (*initialize_event_t)(void);
typedef void (*shutdown_event_t)(void);

// Handle main loop events.
typedef void (*handle_tick_event_t)(float delta_time);

// Interface through which the engine passes events and triggers.
typedef struct game_manager_interface
{
	// Initialize and clean-up events.
	initialize_event_t initialize_event;
	shutdown_event_t shutdown_event;

	// Main loop event.
	handle_tick_event_t tick_event;
} game_manager_interface_t;

#endif // _GAME_MANAGER_INTERFACE_H_