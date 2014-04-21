#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>
#include "opengl_model.h"
#include "polygon.h"
#include "renderer.h"

// Structure representing OpenGL state.
typedef struct opengl_context
{
	GLuint vertex_array;

	// Shader members.
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
} opengl_context_t;

// OpenGL initialization/destruction.
void null_opengl_context(opengl_context_t *state);

// Renderer/OpenGL conversions.
GLenum get_opengl_shader_type(renderer_shader_type_t type);

// Filling out renderer interface.
void initialize_opengl_interface();

// Renderer interface functions.
int initialize_opengl();
void destroy_opengl();
int create_opengl_mesh_model(const mesh_t *mesh,
	renderer_model_t **out);
int create_opengl_indexed_mesh_model(const indexed_mesh_t *indexed_mesh,
	renderer_model_t **out);
void destroy_opengl_model(renderer_model_t *model);
void render_opengl_model(const renderer_model_t *model);
int create_opengl_shader(const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);

#endif // _OPENGL_RENDERER_H_
