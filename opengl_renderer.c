#include "opengl_renderer.h"
#include "file.h"
#include "memory_manager.h"
#include <math.h>
#include <stdio.h>

// Singleton reference for OpenGL renderer.
static opengl_context_t opengl;

// Private functions.
static int get_vertex_size(const renderer_shader_schema_t *schema);
static int get_attribute_size(const renderer_variable_type_t attribute_type);
static int get_num_attribute_floats(const renderer_variable_type_t attribute_type);

/* Null OpenGL state to uninitialized state. */
void opengl_null_context(opengl_context_t *state)
{
	state->active_program = 0;
}

/* Null OpenGL shader to uninitialized state. */
void opengl_null_shader(opengl_shader_t *shader)
{
	shader->handle = 0;
	shader->is_linked = 0;
}

/* Null OpenGL program. */
void opengl_null_program(opengl_program_t *program)
{
	program->handle = 0;
}

/* Null OpenGL shader schema. */
void opengl_null_shader_schema(opengl_shader_schema_t *schema)
{
	schema->attributes = NULL;
	schema->num_attributes = 0;
}

/* Null OpenGL model. */
void opengl_null_model(opengl_model_t *model)
{
	model->array_size = 0;
	model->index_buffer = 0;
	model->vertex_buffer = 0;
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
	// Initialization and destruction.
	renderer->initialize = &opengl_initialize;
	renderer->destroy = &opengl_destroy;

	// Shader functions.
	renderer->create_shader_schema = &opengl_create_shader_schema;
	renderer->destroy_shader_schema = &opengl_destroy_shader_schema;
	renderer->create_shader = &opengl_create_shader;
	renderer->destroy_shader = &opengl_destroy_shader;

	// Shader program functions.
	renderer->create_program = &opengl_create_program;
	renderer->destroy_program = &opengl_destroy_program;
	renderer->link_shader = &opengl_link_shader;
	renderer->compile_program = &opengl_compile_program;
	renderer->set_program = &opengl_set_program;
	renderer->unset_program = &opengl_unset_program;

	// Model functions.
	renderer->create_model = &opengl_create_model;
	renderer->create_indexed_model = &opengl_create_indexed_model;
	renderer->destroy_model = &opengl_destroy_model;
	renderer->draw_model = &opengl_draw_model;

	// Rendering functions.
	renderer->clear_scene = &opengl_clear_scene;
}

/*
* Initialize OpenGL scene.
* The state is partially filled out as components are initialized
* to allow for clean-up.
*/
int opengl_initialize(void)
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
void opengl_destroy(void)
{
}

/*
* Create and compile a shader from a file.
* Returns 1 on success, 0 otherwise.
* Fills out output struct as soon as something is available (so must be cleaned by caller).
*/
int opengl_create_shader(const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out)
{
	opengl_shader_t *shader;
	GLuint shader_handle;
	GLchar *source;
	int compile_status;
	GLsizei log_length;
	GLchar *log;
	GLenum shader_type;

	// Allocate shader structure.
	shader = (opengl_shader_t*)memory_allocate(sizeof(opengl_shader_t));
	if (shader == NULL) {
		return 0;
	}
	opengl_null_shader(shader);
	out->buffer = shader;
	
	// Load shader source.
	if (read_file(filename, &source) == 0) {
		printf("Failed to open shader file '%s'.\n", filename);
		return 0;
	}

	// Create and compile shader.
	shader_type = get_opengl_shader_type(type);
	shader_handle = glCreateShader(shader_type);
	if (shader_handle == 0) {
		printf("Failed to create GL shader for '%s'.\n", filename);
		return 0;
	}
	glShaderSource(shader_handle, 1, (const GLchar**)&source, 0);
	glCompileShader(shader_handle);
	free(source);
	shader->handle = shader_handle;

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
	return 1;
}

/*
* Destroy OpenGL shader.
* Takes in the program that it's attached to.
*/
void destroy_opengl_shader(renderer_shader_t *shader, renderer_program_t program)
{
	opengl_shader_t *opengl_shader;
	opengl_program_t *opengl_program;
	GLuint shader_handle;

	// Check if shader exists at all.
	opengl_shader = (opengl_shader_t*)shader->buffer;
	if (opengl_shader != NULL) {
		// Detach if linked.
		shader_handle = opengl_shader->handle;
		if (opengl_shader->is_linked) {
			opengl_program = (opengl_program_t*)program.buffer;
			glDetachShader(opengl_program->handle, shader_handle);
		}
		glDeleteShader(shader_handle);

		// Destroy structure.
		memory_free(opengl_shader);
		renderer_null_shader(shader);
	}
}

/*
* Create a renderer context for a given mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int create_opengl_model(const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out)
{
	int vertex_size;
	GLuint vertex_buffer;
	opengl_model_t *model;
	opengl_shader_schema_t *opengl_schema;

	// Allocate space for model.
	model = (opengl_model_t*)memory_allocate(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	null_opengl_model(model);
	out->buffer = model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);

	// Load buffer data.
	opengl_schema = (opengl_shader_schema_t*)schema.buffer;
	vertex_size = opengl_schema->vertex_size;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, vertex_data, GL_STATIC_DRAW);

	// Fill out handles and size.
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
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint index_buffer;
	opengl_model_t *model;

	// Attribute setup.
	int i;
	int attribute_size;
	int num_attributes;
	int num_floats;
	GLint location;
	GLchar *offset;
	const renderer_shader_attribute_t *attribute;
	renderer_variable_type_t type;

	// Allocate space for model.
	model = (opengl_model_t*)memory_allocate(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	null_opengl_model(model);
	out->buffer = model;

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

	// Unbind array.
	glBindVertexArray(0);

	// Fill out handles and size.
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
	opengl_model = (opengl_model_t*)model->buffer;
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
	memory_free(opengl_model);
	renderer_null_model(model);
}

/*
 * Clear an OpenGL scene for a new frame.
 */
void opengl_clear_scene(void)
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
		glBindVertexBuffer(GL_VERTEX_ARRAY, opengl_model->vertex_buffer,);
		glBindVertexBuffer(GL_INDEX_ARRAY, opengl_model->index_buffer);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_UNSIGNED_INT, NULL);
	}
	else if (opengl_model->vertex_buffer != 0) {
		glBindVertexArray(opengl_model->vertex_array);
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_FLOAT, NULL);
	}

	glBindVertexArray(0);
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
void set_opengl_shader_program(renderer_program_t program)
{
	// Set program as active.
	
	glUseProgram(gl_program);
	opengl.active_program = gl_program;
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
