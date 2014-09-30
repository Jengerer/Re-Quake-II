#ifndef _OPENGL_EXPORT_H_
#define _OPENGL_EXPORT_H_

#include "shared_defines.h"
#include "renderer.h"
#include "renderer_shader_utilities.h"

// Function to retrieve base renderer functions.
void DLL_EXPORT populate_renderer(renderer_t *renderer);

// Function to retrieve renderer shader utilties.
void DLL_EXPORT populate_renderer_shader_utilities(renderer_shader_utilities_t *utilities);

#endif // _OPENGL_EXPORT_H_