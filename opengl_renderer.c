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
 * Initialize OpenGL scene.
 * The state is partially filled out as components are initialized
 * to allow for clean-up.
 */
int initialize_opengl(opengl_state_t *state)
{
	GLint shader;
	GLint program;
	int link_status;

	// Initialize GLEW.
	glewInit();

	// Create a shader program.
	program = glCreateProgram();
	state->program = program;

	// Create vertex shader.
	shader = create_shader_from_file(VERTEX_SHADER_FILE, GL_VERTEX_SHADER);
	if (shader == 0) {
		printf("Failed to create vertex shader.\n");
		return 0;
	}
	state->vertex_shader = shader;
	glAttachShader(program, shader);

	// Create fragment shader.
	shader = create_shader_from_file(FRAGMENT_SHADER_FILE, GL_FRAGMENT_SHADER);
	if (shader == 0) {
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
void destroy_opengl(opengl_state_t *state)
{
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
	null_opengl_state(state);
}

/*
 * Create and compile a shader from a file.
 * Returns the GL handle to the shader on success, 0 otherwise.
 */
GLuint create_shader_from_file(const char *filename, GLenum shader_type)
{
	GLuint shader;
	GLchar *source;
	int compile_status;
	GLsizei log_length;
	GLchar *log;

	// Create vertex shader.
	if (read_file(filename, &source) == 0) {
		printf("Failed to open shader file '%s'.\n", filename);
		return 0;
	}
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
	return shader;
}

/*
 * Create a renderer context for a given mesh.
 * Returns 1 and fills out struct on success, returns 0 otherwise.
 */
int create_opengl_model(const mesh_t *mesh, opengl_model_t* out)
{
	GLuint vertex_buffer;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(vector3d_t), mesh->vertices, GL_STATIC_DRAW);
	out->vertex_buffer = vertex_buffer;
	return 1;
}
