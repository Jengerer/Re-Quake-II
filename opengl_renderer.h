#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>

// Structure representing OpenGL state.
typedef struct opengl_state
{
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

#endif // _OPENGL_RENDERER_H_
