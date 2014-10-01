#pragma once

// Interface to which the engine sends its events.
class EngineListener
{

public:

	// Triggered when the engine is fully initialized.
	virtual bool OnInitialize() = 0;
	virtual void OnShutdown() = 0;

	// Called on every frame.
	virtual int OnTick() = 0;

};

// Initialization and clean-up events.
typedef void (*engine_null_t)(void);
typedef int (*engine_initialize_t)(void);
typedef void (*engine_shutdown_t)(void);

// Handle main loop events.
typedef int (*engine_tick_t)(float delta_time);

// Interface through which the engine passes events and triggers.
typedef struct engine_listener
{
	// Initialize and clean-up events.
	engine_null_t on_null;
	engine_initialize_t on_initialize;
	engine_shutdown_t on_shutdown;

	// Main loop event.
	engine_tick_t on_tick;
} engine_listener_t;

#endif // _ENGINE_LISTENER_H_