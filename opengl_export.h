#ifndef _OPENGL_EXPORT_H_
#define _OPENGL_EXPORT_H_

#include "shared_defines.h"
#include "renderer.h"

// Function to retrieve renderer functions.
void DLL_EXPORT populate_renderer(renderer_t *renderer);

#endif // _OPENGL_EXPORT_H_