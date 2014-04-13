#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>
#include "opengl_model.h"
#include "polygon.h"
#include "renderer.h"

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

// Renderer/OpenGL conversions.
GLenum get_opengl_shader_type(renderer_shader_type_t type);

// Filling out renderer interface.
void initialize_opengl_interface(renderer_t *renderer);

// Renderer interface functions.
int initialize_opengl(renderer_context_t *context);
void destroy_opengl(renderer_context_t *context);
int create_opengl_model(renderer_context_t *context, const mesh_t *mesh, renderer_model_t *out);
void render_opengl_model(renderer_context_t *context, const renderer_model_t *model);
int create_opengl_shader(renderer_context_t *context,
	const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);

// Engine interface intiailization.

#endif // _OPENGL_RENDERER_H_