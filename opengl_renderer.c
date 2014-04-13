#include "opengl_renderer.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"

/*
 * Create null OpenGL state for clean destruction.
 */
void null_opengl_state(opengl_state_t *state)
{
	state->vertex_shader = 0;
	state->fragment_shader = 0;
	state->program = 0;
}

/*
 * Convert renderer shader type to OpenGL type.
 */
GLenum get_opengl_shader_type(renderer_shader_type_t type)
{
	switch (type) {
	case VERTEX_SHADER:
		return GL_VERTEX_SHADER;
	case FRAGMENT_SHADER:
		return GL_FRAGMENT_SHADER;
	default:
		break;
	}
	fprintf(stderr, "Invalid shader type found!");
	return 0;
}

/*
 * Allocate renderer and fill out interface struct.
 * Fill out a renderer interface with OpenGL functions.
 */
void initialize_opengl_interface(renderer_t *renderer)
{
	// Fill out interface functions.
	renderer->initialize = &initialize_opengl;
	renderer->destroy = &destroy_opengl;
	renderer->create_model = &create_opengl_model;
	renderer->render_model = &render_opengl_model;
	renderer->create_shader = &create_opengl_shader;
}

/*
* Initialize OpenGL scene.
* The state is partially filled out as components are initialized
* to allow for clean-up.
*/
int initialize_opengl(renderer_context_t **out)
{
	GLint shader;
	GLint program;
	int link_status;
	renderer_context_t *context;
	opengl_state_t *state;

	// Allocate OpenGL renderer state.
	state = (opengl_state_t*)malloc(sizeof(opengl_state_t));
	if (state == NULL) {
		return 0;
	}
	null_opengl_state(state);
	context = (renderer_context_t*)state;
	*out = context;

	// Initialize GLEW.
	glewInit();

	// Create a shader program.
	program = glCreateProgram();
	state->program = program;

	// Create vertex shader.
	if (!create_opengl_shader(context, VERTEX_SHADER_FILE, VERTEX_SHADER, &shader)) {
		printf("Failed to create vertex shader.\n");
		return 0;
	}
	state->vertex_shader = shader;
	glAttachShader(program, shader);

	// Create fragment shader.
	if (!create_opengl_shader(context, FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, &shader)) {
		printf("Failed to create fragment shader.\n");
		return 0;
	}
	state->fragment_shader = shader;
	glAttachShader(program, shader);

	// Bind attributes to variable names.
	glBindAttribLocation(program, 0, "in_pos");
	glBindAttribLocation(program, 1, "in_colour");

	// Link the shader program and check that it succeeded.
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		printf("Failed to link shader program.\n");
		return 0;
	}
	glUseProgram(program);
	return 1;
}

/*
* Deallocate OpenGL context.
*/
void destroy_opengl(renderer_context_t *context)
{
	opengl_state_t *state = (opengl_state_t*)context;

	// Check if we even started initializing.
	if (state == NULL) {
		return;
	}

	glUseProgram(0);

	// Deallocate vertex shader.
	if (state->vertex_shader != 0) {
		glDetachShader(state->program, state->vertex_shader);
		glDeleteShader(state->vertex_shader);
	}

	// Deallocate fragment shader.
	if (state->fragment_shader != 0) {
		glDetachShader(state->program, state->fragment_shader);
		glDeleteShader(state->fragment_shader);
	}

	// Destroy the program.
	if (state->program != 0) {
		glDeleteProgram(state->program);
	}
	free(state);
}

/*
* Create a renderer context for a given mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_model(renderer_context_t *context, const mesh_t *mesh, renderer_model_t *out)
{
	GLuint vertex_buffer;
	opengl_model_t *model = (opengl_model_t*)out;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(vector3d_t), mesh->vertices, GL_STATIC_DRAW);
	model->vertex_buffer = vertex_buffer;
	return 1;
}

/*
* Render an OpenGL model.
*/
void render_opengl_model(renderer_context_t *context, const renderer_model_t *model)
{
}

/*
* Create and compile a shader from a file.
* Returns 1 on success, 0 otherwise.
* Fills out the output struct as soon as it can be deallocated.
*/
int create_opengl_shader(renderer_context_t *context,
	const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out)
{
	GLuint shader;
	GLchar *source;
	int compile_status;
	GLsizei log_length;
	GLchar *log;
	GLenum shader_type;
	GLuint *shader_ptr;

	// Load shader source.
	if (read_file(filename, &source) == 0) {
		printf("Failed to open shader file '%s'.\n", filename);
		return 0;
	}

	// Create and compile shader.
	shader_type = get_opengl_shader_type(type);
	shader = glCreateShader(shader_type);
	if (shader == 0) {
		printf("Failed to create GL shader for '%s'.\n", filename);
		return 0;
	}
	glShaderSource(shader, 1, (const GLchar**)&source, 0);
	glCompileShader(shader);
	free(source);

	// Check that it compiled properly.
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		// Print error.
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		log = (char*)malloc(log_length * sizeof(GLchar));
		glGetShaderInfoLog(shader, log_length, &log_length, log);
		fprintf(stderr, "%s\n", log);
		free(log);

		glDeleteShader(shader);
		printf("Failed to compile shader '%s'.\n", filename);
		return 0;
	}

	// Fill output struct.
	shader_ptr = (GLuint*)out;
	*shader_ptr = shader;
	return 1;
}
