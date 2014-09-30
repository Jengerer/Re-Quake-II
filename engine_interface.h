#ifndef _ENGINE_INTERFACE_H_
#define _ENGINE_INTERFACE_H_

// Engine functions.
typedef int (*engine_initialize_t)(void);
typedef void (*engine_destroy_t)(void);
typedef int (*engine_run_t)(void);

// Interface for the application to initialize, run, and shut down the engine.
typedef struct engine_interface
{
	engine_initialize_t initialize;
	engine_shutdown_t shutdown;
	engine_run_t run;
} engine_interface_t;

#endif // _ENGINE_INTERFACE_H_