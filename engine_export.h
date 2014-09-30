#ifndef _ENGINE_EXPORT_H_
#define _ENGINE_EXPORT_H_

#include "shared_defines.h"
#include "engine_interface.h"
#include "engine_listener.h"
#include "engine_utilities.h"
#include "renderer.h"
#include "renderer_shader_utilities.h"

// Request engine to populate structure for engine application interface.
void DLL_EXPORT engine_populate_interface(engine_interface_t *engine_interface);

// Request engine to populate structure with engine utility functions.
void DLL_EXPORT engine_populate_utilities(engine_utilities_t *utilities);

// Retrieve the struct to fill out for engine listener.
engine_listener_t *DLL_EXPORT engine_get_listener_destination(void);

// Retrieve the struct to fill with renderer interface.
renderer_t *DLL_EXPORT engine_get_renderer_destination(void);

// Retrieve the struct to fill with renderer shader uilities.
renderer_shader_utilities_t *DLL_EXPORT engine_get_shader_utilities_destination(void);

#endif // _ENGINE_EXPORT_H_