#include "opengl_renderer.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"

// Singleton reference for OpenGL renderer.
opengl_state_t opengl_state;

/*
 * Create null OpenGL state for clean destruction.
 */
void null_opengl_context(opengl_context_t *state)
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
	// Null OpenGL state here for clean up.
	null_opengl_context(&opengl_state);

	// Fill out interface functions.
	renderer->initialize = &initialize_opengl;
	renderer->destroy = &destroy_opengl;
	renderer->create_mesh_model = &create_opengl_mesh_model;
	renderer->create_indexed_mesh_model = &create_opengl_indexed_mesh_model;
	renderer->destroy_model = &destroy_opengl_model;
	renderer->render_model = &render_opengl_model;
	renderer->create_shader = &create_opengl_shader;
}

/*
* Initialize OpenGL scene.
* The state is partially filled out as components are initialized
* to allow for clean-up.
*/
int initialize_opengl()
{
	GLint shader;
	GLint program;
	int link_status;

	// Initialize GLEW.
	glewInit();

	// Create a shader program.
	program = glCreateProgram();
	opengl_state.program = program;

	// Create vertex shader.
	if (!create_opengl_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, &shader)) {
		printf("Failed to create vertex shader.\n");
		return 0;
	}
	opengl_state.vertex_shader = shader;
	glAttachShader(program, shader);

	// Create fragment shader.
	if (!create_opengl_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, &shader)) {
		printf("Failed to create fragment shader.\n");
		return 0;
	}
	opengl_state->fragment_shader = shader;
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
void destroy_opengl()
{
	// Unset program so we can detach.
	if (state->program != 0) {
		glUseProgram(0);
	}

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
}

/*
* Create a renderer context for a given mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_mesh_model(const mesh_t *mesh, renderer_model_t **out)
{
	GLuint vertex_buffer;
	opengl_model_t *model = (opengl_model_t*)malloc(sizeof(opengl_model_t));
	null_opengl_model(model);
	*out = (renderer_model_t*)model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(vector3d_t), mesh->vertices, GL_STATIC_DRAW);
	model->vertex_buffer = vertex_buffer;
	model->array_size = mesh->num_vertices;
	return 1;
}

/*
* Create a renderer context for a given indexed mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_indexed_mesh_model(const indexed_mesh_t *indexed_mesh, renderer_model_t **out)
{
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;
	const mesh_t *mesh;
	opengl_model_t *model;
	mesh = &indexed_mesh->mesh;

	// Allocate space for the model.
	model = (opengl_model_t*)malloc(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	null_opengl_model(model);
	*out = (renderer_model_t*)model;

	// Allocate vertex array object and bind.
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(mesh_vertex_t), mesh->vertices, GL_STATIC_DRAW);

	// Set attributes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex_t), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex_t), (GLvoid*)sizeof(vector3d_t));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Create index buffer.
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexed_mesh->num_indices * sizeof(unsigned int), indexed_mesh->indices, GL_STATIC_DRAW);
	
	// Unbind VAO.
	glBindVertexArray(0);

	// Create index buffer.
	model->vertex_array = vertex_array;
	model->vertex_buffer = vertex_buffer;
	model->index_buffer = index_buffer;
	model->array_size = indexed_mesh->num_indices;
	return 1;
}

/*
 * Destroy the OpenGL model.
 */
void destroy_opengl_model(renderer_model_t *model)
{
	opengl_model_t *opengl_model;

	// Check if anything was actually allocated.
	if (model == NULL) {
		return;
	}
	opengl_model = (opengl_model_t*)model;

	// Free vertex buffer if there is one.
	if (opengl_model->vertex_buffer != 0) {
		glDeleteBuffers(1, &opengl_model->vertex_buffer);

		// We won't have index buffer if we have no vertex buffer.
		if (opengl_model->index_buffer != 0) {
			glDeleteBuffers(1, &opengl_model->index_buffer);
		}
	}
	
	// Free the struct.
	free(opengl_model);
}

/*
* Render an OpenGL model.
*/
void render_opengl_model(const renderer_model_t *model)
{
	const opengl_model_t *opengl_model = (const opengl_model_t*)model;

	// Draw differently depending on indexed or not.
	if (opengl_model->index_buffer != 0) {
		glBindVertexArray(opengl_model->vertex_array);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_UNSIGNED_INT, NULL);
	}
	else if (opengl_model->vertex_buffer != 0) {
		glBindVertexArray(opengl_model->vertex_array);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_FLOAT, NULL);
	}
}

/*
* Create and compile a shader from a file.
* Returns 1 on success, 0 otherwise.
* Fills out the output struct as soon as it can be deallocated.
*/
int create_opengl_shader(const char *filename,
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

