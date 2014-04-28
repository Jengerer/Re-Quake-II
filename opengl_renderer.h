#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include <GL/glew.h>
#include "opengl_model.h"
#include "polygon.h"
#include "renderer.h"

// Structure representing OpenGL state.
typedef struct opengl_context
{
	GLuint active_program;
} opengl_context_t;

// OpenGL initialization/destruction.
void null_opengl_context(opengl_context_t *state);

// Renderer/OpenGL conversions.
GLenum get_opengl_shader_type(renderer_shader_type_t type);

// Filling out renderer interface.
void initialize_opengl_interface();

// Renderer interface functions.
int initialize_opengl(void);
void destroy_opengl(void);
int create_opengl_model(const void *vertex_data,
	int num_vertices,
	const renderer_shader_schema_t *schema,
	renderer_model_t *out);
int create_opengl_indexed_model(const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	const renderer_shader_schema_t *schema,
	renderer_model_t *out);
void destroy_opengl_model(renderer_model_t *model);
void clear_opengl_scene(void);
void render_opengl_model(const renderer_model_t *model);
int create_opengl_shader_program(renderer_shader_program_t *out, const renderer_shader_schema_t *schema);
void destroy_opengl_shader_program(renderer_shader_program_t *program);
int create_opengl_shader(const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out);
void destroy_opengl_shader(renderer_shader_t *shader, renderer_shader_program_t *program);
void link_opengl_shader(renderer_shader_t shader, renderer_shader_program_t *program);
int compile_opengl_shader_program(renderer_shader_program_t *program);
void set_opengl_shader_program(renderer_shader_program_t *program);
void unset_opengl_shader_program(void);

#endif // _OPENGL_RENDERER_H_