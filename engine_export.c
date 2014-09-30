#include "engine_export.h"
#include "engine.h"

// Populate engine interface for application to use.
void populate_engine_interface(engine_interface_t *engine_interface)
{
	engine_interface->initialize = &engine_initialize;
	engine_interface->shutdown = &engine_shutdown;
	engine_interface->run = &engine_run;
}
