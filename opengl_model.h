#ifndef _OPENGL_MODEL_H_
#define _OPENGL_MODEL_H_

#include <GL/glew.h>

// Representation of a mesh for rendering in OpenGL.
typedef struct opengl_model
{
	GLuint vertex_buffer;
} opengl_model_t;

// Only null function here; initialization/destruction responsibility of renderer.
void null_opengl_model(opengl_model_t *model);

#endif // _OPENGL_MODEL_H_