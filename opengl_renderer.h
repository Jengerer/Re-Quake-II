#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>
#include "opengl_model.h"
#include "polygon.h"

// Structure representing OpenGL state.
typedef struct opengl_state
{
	GLuint vertex_array;

	// Shader members.
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
} opengl_state_t;

// OpenGL initialization/destruction.
void null_opengl_state(opengl_state_t *state);
int initialize_opengl(opengl_state_t *state);
void destroy_opengl(opengl_state_t *state);

// Create a shader from a shader file.
GLuint create_shader_from_file(const char *filename, GLenum shader_type);

// Create a renderer context for a given mesh.
int create_opengl_model(const mesh_t *mesh, opengl_model_t* out);

#endif // _OPENGL_RENDERER_H_
