#ifndef _OPENGL_MODEL_H_
#define _OPENGL_MODEL_H_

#include <GL/gl.h>

// Representation of a mesh for rendering in OpenGL.
typedef struct opengl_model
{
	GLuint vertex_buffer;
} opengl_model_t;

#endif // _OPENGL_MODEL_H_