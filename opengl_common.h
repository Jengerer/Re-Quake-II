#ifndef _OPENGL_COMMON_H_
#define _OPENGL_COMMON_H_

#include <GL/glew.h>
#include "renderer_shared.h"

// OpenGL renderer context.
typedef struct opengl_context
{
	// Put frame buffers here and such.
	int unused;
} opengl_context_t;

// Singleton OpenGL context.
extern opengl_context_t opengl;

#endif // _OPENGL_COMMON_H_