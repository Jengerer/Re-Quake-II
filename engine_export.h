#ifndef _ENGINE_EXPORT_H_
#define _ENGINE_EXPORT_H_

#include "shared_defines.h"
#include "engine_interface.h"
#include "engine_listener.h"
#include "engine_utilities.h"

// Request engine to populate structure for engine application interface.
void DLL_EXPORT populate_engine_interface(engine_interface_t *engine);

// Request engine to populate structure with engine utility functions.
void DLL_EXPORT populate_engine_utilities(engine_utilities_t *utilities);

// Retrieve the struct to fill out for engine listener.
engine_listener_t *DLL_EXPORT get_engine_listener(void);

#endif // _ENGINE_EXPORT_H_