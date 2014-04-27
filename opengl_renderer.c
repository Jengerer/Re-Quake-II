#include "opengl_renderer.h"
#include "file.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Singleton reference for OpenGL renderer.
static opengl_context_t opengl;

// Private functions.
static int get_vertex_size(const renderer_shader_schema_t *schema);
static int get_attribute_size(const renderer_shader_attribute_type_t attribute_type);
static int get_num_attribute_floats(const renderer_shader_attribute_type_t attribute_type);

/*
 * Create null OpenGL state for clean destruction.
 */
void null_opengl_context(opengl_context_t *state)
{
	(void)state;
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
	null_opengl_context(&opengl);

	// Fill out interface functions.
	renderer->initialize = &initialize_opengl;
	renderer->destroy = &destroy_opengl;
	renderer->create_model = &create_opengl_model;
	renderer->create_indexed_model = &create_opengl_indexed_model;
	renderer->destroy_model = &destroy_opengl_model;
	renderer->clear_scene = &clear_opengl_scene;
	renderer->render_model = &render_opengl_model;
	renderer->create_shader_program = &create_opengl_shader_program;
	renderer->destroy_shader_program = &destroy_opengl_shader_program;
	renderer->create_shader = &create_opengl_shader;
	renderer->destroy_shader = &destroy_opengl_shader;
	renderer->link_shader = &link_opengl_shader;
	renderer->compile_shader_program = &compile_opengl_shader_program;
	renderer->set_shader_program = &set_opengl_shader_program;
	renderer->unset_shader_program = &unset_opengl_shader_program;
}

/*
* Initialize OpenGL scene.
* The state is partially filled out as components are initialized
* to allow for clean-up.
*/
int initialize_opengl(void)
{
	// Initialize GLEW.
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Unable to initialize GLEW: %s.\n", glewGetErrorString(error));
		return 0;
	}

	// Set up GL rendering parameters.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return 1;
}

/*
* Deallocate OpenGL context.
*/
void destroy_opengl(void)
{
}

/*
* Create a renderer context for a given mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_model(const void *vertex_data,
	int num_vertices,
	const renderer_shader_schema_t *schema,
	renderer_model_t *out)
{
	int vertex_size;
	GLuint vertex_buffer;
	opengl_model_t *model;

	// Allocate space for model.
	model = (opengl_model_t*)malloc(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	null_opengl_model(model);

	// Fill out allocated space.
	out->reference.as_pointer = model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);

	// Load buffer data.
	vertex_size = get_vertex_size(schema);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, vertex_data, GL_STATIC_DRAW);

	model->vertex_buffer = vertex_buffer;
	model->array_size = num_vertices;
	return 1;
}

/*
* Create a renderer context for a given indexed mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_indexed_model(const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	const renderer_shader_schema_t *schema,
	renderer_model_t *out)
{
	int vertex_size;
	GLuint vertex_buffer;
	GLuint index_buffer;
	opengl_model_t *model;

	// Allocate space for model.
	model = (opengl_model_t*)malloc(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	null_opengl_model(model);

	// Fill out allocated space.
	out->reference.as_pointer = model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);

	// Load buffer data.
	vertex_size = get_vertex_size(schema);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, vertex_data, GL_STATIC_DRAW);

	// Create index buffer.
	glGenBuffers(1, &index_buffer);

	// Load index data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

	// Unbind buffers.
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	model->vertex_buffer = vertex_buffer;
	model->index_buffer = index_buffer;
	model->array_size = num_indices;
	return 1;
}

/*
 * Destroy the OpenGL model.
 */
void destroy_opengl_model(renderer_model_t *model)
{
	GLuint vertex_buffer;
	GLuint index_buffer;
	opengl_model_t *opengl_model;

	// Check if anything was actually allocated.
	opengl_model = (opengl_model_t*)model->reference.as_pointer;
	if (opengl_model == NULL) {
		return;
	}

	// Free vertex buffer if there is one.
	vertex_buffer = opengl_model->vertex_buffer;
	if (vertex_buffer != 0) {
		glDeleteBuffers(1, &vertex_buffer);

		// We won't have index buffer if we have no vertex buffer.
		index_buffer = opengl_model->index_buffer;
		if (index_buffer != 0) {
			glDeleteBuffers(1, &index_buffer);
		}
	}
	
	// Free the struct.
	free(opengl_model);
}

/*
 * Clear an OpenGL scene for a new frame.
 */
void clear_opengl_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

/*
* Render an OpenGL model.
*/
void render_opengl_model(const renderer_model_t *model)
{
	const opengl_model_t *opengl_model = (const opengl_model_t*)model->reference.as_pointer;

	// Draw differently depending on indexed or not.
	if (opengl_model->index_buffer != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, opengl_model->vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_model->index_buffer);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_UNSIGNED_INT, NULL);
	}
	else if (opengl_model->vertex_buffer != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, opengl_model->vertex_buffer);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_FLOAT, NULL);
	}
}

/*
 * Create an OpenGL shader program.
 */
int create_opengl_shader_program(renderer_shader_program_t *out, const renderer_shader_schema_t *schema)
{
	GLuint program;
	
	// Create shader object.
	program = glCreateProgram();
	if (program == 0) {
		fprintf(stderr, "Failed to create shader program.\n");
		return 0;
	}

	out->reference.as_integer = (unsigned int)program;
	out->schema = schema;
	return 1;
}

/*
 * Destroy an OpenGL shader program.
 */
void destroy_opengl_shader_program(renderer_shader_program_t *program)
{
	GLuint shader_program = (GLuint)program->reference.as_integer;
	if (shader_program != 0) {
		glDeleteProgram(shader_program);
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

	// Print error/warning.
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length != 0) {
		log = (char*)malloc(log_length * sizeof(GLchar));
		if (log != NULL) {
			glGetShaderInfoLog(shader, log_length, &log_length, log);
			fprintf(stderr, "%s\n", log);
			free(log);
		}
	}

	// Check that it compiled properly.
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		printf("Failed to compile shader '%s'.\n", filename);
		return 0;
	}

	// Fill output struct.
	out->reference.as_integer = (unsigned int)shader;
	return 1;
}

/*
 * Destroy OpenGL shader.
 * Takes in the program that it's attached to.
 */
void destroy_opengl_shader(renderer_shader_t *shader, renderer_shader_program_t *program)
{
	GLuint gl_program;
	GLuint gl_shader;
	
	// Detach and delete.
	gl_shader = (GLuint)shader->reference.as_integer;
	gl_program = (GLuint)program->reference.as_integer;
	if (gl_shader != 0) {
		glDetachShader(gl_program, gl_shader);
		glDeleteShader(gl_shader);
		shader->reference.as_integer = 0;
	}
}

/*
 * Link a shader to a program.
 */
void link_opengl_shader(renderer_shader_t shader, renderer_shader_program_t *program)
{
	GLuint gl_shader;
	GLuint gl_program;

	// Link the shader to the program.
	gl_shader = (GLuint)shader.reference.as_integer;
	gl_program = (GLuint)program->reference.as_integer;
	glAttachShader(gl_program, gl_shader);
}

/*
 * Link and compile an OpenGL shader program.
 */
int compile_opengl_shader_program(renderer_shader_program_t *program)
{
	char *log;
	GLint log_length;
	GLint link_status;
	GLuint gl_program;

	// Link program and check status.
	gl_program = (GLuint)program->reference.as_integer;
	glLinkProgram(gl_program);

	// Print error/warning.
	glGetProgramiv(gl_program, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length != 0) {
		log = (char*)malloc(log_length * sizeof(GLchar));
		if (log != NULL) {
			glGetProgramInfoLog(gl_program, log_length, &log_length, log);
			fprintf(stderr, "%s\n", log);
			free(log);
		}
	}

	glGetProgramiv(gl_program, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		printf("Failed to link shader program.\n");
		return 0;
	}
	return 1;
}

/*
 * Set the program to be used in rendering.
 */
void set_opengl_shader_program(renderer_shader_program_t *program)
{
	int i;
	int vertex_size;
	int attribute_offset;
	int attribute_size;
	int attribute_location;
	int num_floats;
	int num_attributes;
	const renderer_shader_schema_t *schema;
	const renderer_shader_attribute_t *attribute;
	renderer_shader_attribute_type_t attribute_type;

	// Set program as active.
	GLuint gl_program = (GLuint)program->reference.as_integer;
	glUseProgram(gl_program);
	
	// Set up attributes for active shader.
	schema = program->schema;
	attribute_offset = 0;
	num_attributes = schema->num_attributes;
	vertex_size = get_vertex_size(schema);
	for (i = 0; i < num_attributes; ++i) {
		attribute = &schema->attributes[i];
		attribute_type = attribute->type;
		attribute_size = get_attribute_size(attribute_type);
		attribute_location = glGetAttribLocation(gl_program, attribute->name);
		num_floats = get_num_attribute_floats(attribute_type);
		glEnableVertexAttribArray(attribute_location);
		glVertexAttribPointer(attribute_location, num_floats, GL_FLOAT, GL_FALSE, 0, (GLvoid*)attribute_offset);
		attribute_offset += attribute_size;
	}
}

/*
 * Unset a program from rendering.
 */
void unset_opengl_shader_program(void)
{
	glUseProgram(0);
}

/*
 * Get the size of a full vertex by its schema.
 */
static int get_vertex_size(const renderer_shader_schema_t *schema)
{
	int i;
	int size;
	int num_attributes;
	const renderer_shader_attribute_t *attribute;
	renderer_shader_attribute_type_t attribute_type;

	// Sum up all attribute sizes.
	size = 0;
	num_attributes = schema->num_attributes;
	for (i = 0; i < num_attributes; ++i) {
		attribute = &schema->attributes[i];
		attribute_type = attribute->type;
		size += get_attribute_size(attribute_type);
	}
	return size;
}

/*
 * Get the total size of a given attribute type.
 */
static int get_attribute_size(const renderer_shader_attribute_type_t attribute_type)
{
	return sizeof(float) * get_num_attribute_floats(attribute_type);
}

/*
 * Get number of floats that are in a given attribute type.
 */
static int get_num_attribute_floats(const renderer_shader_attribute_type_t attribute_type)
{
	// Get number of floats per attribute type.
	switch (attribute_type)
	{
	case ATTRIBUTE_FLOAT:
		return 1;
	
	case ATTRIBUTE_TEXTURE_COORDINATE:
	case ATTRIBUTE_VERTEX_2D:
		return 2;

	case ATTRIBUTE_VERTEX_3D:
		return 3;

	case ATTRIBUTE_VERTEX_4D:
		return 4;

	default:
		break;
	}

	return 0;
}
