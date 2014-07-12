#include "opengl_renderer.h"
#include "file.h"
#include "memory_manager.h"
#include <math.h>
#include <stdio.h>

// Singleton reference for OpenGL renderer.
static opengl_context_t opengl;

// Private functions.
static int get_num_variable_floats(const renderer_variable_type_t variable_type);

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

/* Null OpenGL uniform variable structure. */
void opengl_null_uniform(opengl_uniform_t *uniform)
{
	uniform->location = -1;
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
	opengl_null_context(&opengl);

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

	// Shader attribute and variable functions.
	renderer->create_shader_schema = &opengl_create_shader_schema;
	renderer->destroy_shader_schema = &opengl_destroy_shader_schema;
	renderer->get_uniform = &opengl_get_uniform;
	renderer->set_uniform_vector3d = &opengl_set_uniform_vector3d;
	renderer->set_uniform_vector4d = &opengl_set_uniform_vector4d;
	renderer->set_uniform_matrix3x3 = &opengl_set_uniform_matrix3x3;
	renderer->set_uniform_matrix4x4 = &opengl_set_uniform_matrix4x4;

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
	memory_free(source);
	shader->handle = shader_handle;

	// Print error/warning.
	glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length != 0) {
		log = (char*)memory_allocate(log_length * sizeof(GLchar));
		if (log != NULL) {
			glGetShaderInfoLog(shader_handle, log_length, &log_length, log);
			fprintf(stderr, "%s\n", log);
			memory_free(log);
		}
	}

	// Check that it compiled properly.
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);
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
void opengl_destroy_shader(renderer_shader_t *shader, renderer_program_t program)
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

/* Create an OpenGL shader program. */
int opengl_create_program(renderer_program_t *out)
{
	opengl_program_t *opengl_program;
	GLuint program_handle;

	// Allocate space for the structure.
	opengl_program = (opengl_program_t*)memory_allocate(sizeof(opengl_program_t));
	if (opengl_program == NULL) {
		return 0;
	}
	opengl_null_program(opengl_program);
	out->buffer = opengl_program;

	// Create OpenGL program handle.
	program_handle = glCreateProgram();
	if (program_handle == 0) {
		return 0;
	}
	opengl_program->handle = program_handle;
	return 1;
}

/* Clean up OpenGL shader program. */
void opengl_destroy_program(renderer_program_t *out)
{
	opengl_program_t *opengl_program;
	GLuint program_handle;

	// Check if anything to free.
	opengl_program = out->buffer;
	if (opengl_program != NULL) {
		// Free the program handle if there is one.
		program_handle = opengl_program->handle;
		if (program_handle != 0) {
			glDeleteProgram(program_handle);
		}

		// Free the structure.
		memory_free(opengl_program);
		renderer_null_program(out);
	}
}

/* Link a shader to a program. */
void opengl_link_shader(renderer_shader_t shader, renderer_program_t program)
{
	opengl_shader_t *opengl_shader;
	opengl_program_t *opengl_program;
	GLuint shader_handle;
	GLuint program_handle;

	// Link the shader to the program.
	opengl_shader = (opengl_shader_t*)shader.buffer;
	opengl_program = (opengl_program_t*)program.buffer;
	shader_handle = opengl_shader->handle;
	program_handle = opengl_program->handle;
	glAttachShader(program_handle, shader_handle);
}

/*
 * Link and compile an OpenGL shader program.
 */
int opengl_compile_program(renderer_program_t program)
{
	opengl_program_t *opengl_program;
	char *log;
	GLint log_length;
	GLint link_status;
	GLuint program_handle;

	// Link program and check status.
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	glLinkProgram(program_handle);

	// Print error/warning.
	glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length != 0) {
		log = (char*)memory_allocate(log_length * sizeof(GLchar));
		if (log != NULL) {
			glGetProgramInfoLog(program_handle, log_length, &log_length, log);
			fprintf(stderr, "%s\n", log);
			memory_free(log);
		}
	}

	glGetProgramiv(program_handle, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		printf("Failed to link shader program.\n");
		return 0;
	}
	return 1;
}

/*
 * Set the program to be used in rendering.
 */
void opengl_set_program(renderer_program_t program)
{
	opengl_program_t *opengl_program;
	GLuint program_handle;

	// Set program as active.
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	glUseProgram(program_handle);
	opengl.active_program = program_handle;
}

/*
 * Unset a program from rendering.
 */
void opengl_unset_program(void)
{
	glUseProgram(0);
}

/* Generate an OpenGL shader schema for generic renderer schema. */
int opengl_create_shader_schema(
	renderer_program_t program,
	const renderer_shader_attribute_t *attributes,
	int num_attributes,
	renderer_shader_schema_t *out)
{
	opengl_program_t *opengl_program;
	opengl_shader_schema_t *opengl_schema;
	opengl_shader_attribute_t *opengl_attributes;
	opengl_shader_attribute_t *current_attribute;
	const renderer_shader_attribute_t *attribute;
	GLuint program_handle;
	GLint location;
	GLsizei vertex_size;
	GLint num_floats;
	int i;

	// Allocate space for OpenGL schema and attributes.
	opengl_schema = (opengl_shader_schema_t*)memory_allocate(sizeof(opengl_shader_schema_t));
	if (opengl_schema == NULL) {
		return 0;
	}
	opengl_null_shader_schema(opengl_schema);
	out->buffer = opengl_schema;

	// Allocate space for the attributes.
	opengl_attributes = (opengl_shader_attribute_t*)memory_array_allocate(sizeof(opengl_shader_attribute_t), num_attributes);
	if (opengl_attributes == NULL) {
		return 0;
	}
	opengl_schema->attributes = opengl_attributes;
	opengl_schema->num_attributes = num_attributes;

	// Fill out attribute information.
	vertex_size = 0;
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	for (i = 0; i < num_attributes; ++i) {
		attribute = &attributes[i];
		current_attribute = &opengl_attributes[i];

		// Get attribute data and fill out data.
		num_floats = get_num_variable_floats(attribute->type);
		location = glGetAttribLocation(program_handle, attribute->name);
		current_attribute->location = location;
		current_attribute->num_floats = num_floats;
		current_attribute->offset = (GLchar*)vertex_size;
		vertex_size += num_floats * sizeof(float);
	}
	opengl_schema->vertex_size = vertex_size;
	return 1;
}

/* Destroy the OpenGL schema structure. */
void opengl_destroy_shader_schema(renderer_shader_schema_t *schema)
{
	opengl_shader_schema_t *opengl_schema;

	// Check if anything to clean up.
	opengl_schema = (opengl_shader_schema_t*)schema->buffer;
	if (opengl_schema != NULL) {
		// Check if we should free attributes.
		if (opengl_schema->num_attributes != 0) {
			memory_free(opengl_schema->attributes);
		}
		memory_free(opengl_schema);
		renderer_null_shader_schema(schema);
	}
}

/* Return a handle to an OpenGL shader uniform variable. */
int opengl_get_uniform(
	renderer_program_t program,
	const char *name,
	renderer_uniform_t *out)
{
	opengl_program_t *opengl_program;
	opengl_uniform_t *opengl_uniform;
	GLuint program_handle;
	GLint location;

	// Allocate space for the structure.
	opengl_uniform = (opengl_uniform_t*)memory_allocate(sizeof(opengl_uniform_t));
	if (opengl_uniform == NULL) {
		return 0;
	}
	opengl_null_uniform(opengl_uniform);
	out->buffer = opengl_uniform;

	// Find the variable location.
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	location = glGetUniformLocation(program_handle, (const GLchar*)name);
	if (location == -1) {
		return 0;
	}
	opengl_uniform->location = location;
	return 1;
}

/* Free a handle to a uniform variable. */
void opengl_destroy_uniform(renderer_uniform_t *out)
{
	opengl_uniform_t *opengl_uniform;

	// Check if there's anything to destroy.
	opengl_uniform = (opengl_uniform_t*)out->buffer;
	if (opengl_uniform != NULL) {
		memory_free(opengl_uniform);
		renderer_null_uniform(out);
	}
}

/* Set the value of a 3D vector uniform variable. */
void opengl_set_uniform_vector3d(
	renderer_uniform_t uniform,
	const vector3d_t *vector)
{
	opengl_uniform_t *opengl_uniform;
	GLint location;

	// Vector struct is just three floats, so should be compatible with float[3].
	opengl_uniform = (opengl_uniform_t*)uniform.buffer;
	location = opengl_uniform->location;
	glUniform3fv(location, 1, (const GLfloat*)vector);
}

/* Set the value of a 4D vector uniform variable. */
void opengl_set_uniform_vector4d(
	renderer_uniform_t uniform,
	const vector4d_t *vector)
{
	opengl_uniform_t *opengl_uniform;
	GLint location;

	// Vector struct is just four floats, so should be compatible with float[4].
	opengl_uniform = (opengl_uniform_t*)uniform.buffer;
	location = opengl_uniform->location;
	glUniform4fv(location, 1, (const GLfloat*)vector);
}

/* Set the value of a 3x3 matrix uniform variable. */
void opengl_set_uniform_matrix3x3(
	renderer_uniform_t uniform,
	const matrix3x3_t *matrix)
{
	opengl_uniform_t *opengl_uniform;
	GLint location;

	// Copy the matrix array to the shader variable.
	opengl_uniform = (opengl_uniform_t*)uniform.buffer;
	location = opengl_uniform->location;
	glUniformMatrix3fv(location, 1, GL_TRUE, (const GLfloat*)matrix->array);
}

/* Set the value of a 4x4 matrix uniform variable. */
void opengl_set_uniform_matrix4x4(
	renderer_uniform_t uniform,
	const matrix4x4_t *matrix)
{
	opengl_uniform_t *opengl_uniform;
	GLint location;

	// Copy the matrix array to the shader variable.
	opengl_uniform = (opengl_uniform_t*)uniform.buffer;
	location = opengl_uniform->location;
	glUniformMatrix4fv(location, 1, GL_TRUE, (const GLfloat*)matrix->array);
}

/*
* Create a renderer context for a given mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int opengl_create_model(const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out)
{
	opengl_model_t *model;
	opengl_shader_schema_t *opengl_schema;
	int vertex_size;
	GLuint vertex_buffer;

	// Allocate space for model.
	model = (opengl_model_t*)memory_allocate(sizeof(opengl_model_t));
	if (model == NULL) {
		return 0;
	}
	opengl_null_model(model);
	out->buffer = model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);

	// Load buffer data.
	opengl_schema = (opengl_shader_schema_t*)schema.buffer;
	vertex_size = opengl_schema->vertex_size;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Fill out handles and size.
	model->vertex_buffer = vertex_buffer;
	model->array_size = num_vertices;
	return 1;
}

/*
* Create a renderer context for a given indexed mesh.
* Returns 1 and fills out struct on success, returns 0 otherwise.
*/
int opengl_create_indexed_model(const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	renderer_shader_schema_t schema,
	renderer_model_t *out)
{
	opengl_model_t *opengl_model;
	opengl_shader_schema_t *opengl_schema;
	int vertex_size;
	GLuint vertex_buffer;
	GLuint index_buffer;

	// Allocate space for model.
	opengl_model = (opengl_model_t*)memory_allocate(sizeof(opengl_model_t));
	if (opengl_model == NULL) {
		return 0;
	}
	opengl_null_model(opengl_model);
	out->buffer = opengl_model;

	// Create vertex buffer.
	glGenBuffers(1, &vertex_buffer);
	opengl_model->vertex_buffer = vertex_buffer;

	// Load buffer data.
	opengl_schema = (opengl_shader_schema_t*)schema.buffer;
	vertex_size = opengl_schema->vertex_size;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create index buffer.
	glGenBuffers(1, &index_buffer);
	opengl_model->index_buffer = index_buffer;

	// Load index data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), index_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Fill out handles and size.
	opengl_model->array_size = num_indices;
	return 1;
}

/*
 * Destroy the OpenGL model.
 */
void opengl_destroy_model(renderer_model_t *model)
{
	GLuint vertex_buffer;
	GLuint index_buffer;
	opengl_model_t *opengl_model;
	
	// Check if anything was actually allocated.
	opengl_model = (opengl_model_t*)model->buffer;
	if (opengl_model != NULL) {
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
void opengl_draw_model(renderer_model_t model, renderer_shader_schema_t schema)
{
	const opengl_model_t *opengl_model;
	const opengl_shader_schema_t *opengl_schema;
	const opengl_shader_attribute_t *opengl_attribute;
	GLint location;
	GLsizei vertex_size;
	int i;
	int num_attributes;

	// Draw differently depending on indexed or not.
	opengl_model = (const opengl_model_t*)model.buffer;
	opengl_schema = (const opengl_shader_schema_t*)schema.buffer;
	vertex_size = opengl_schema->vertex_size;
	if (opengl_model->index_buffer != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, opengl_model->vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_model->index_buffer);

		// Set up attributes from schema.
		num_attributes = opengl_schema->num_attributes;
		for (i = 0; i < num_attributes; ++i) {
			opengl_attribute = &opengl_schema->attributes[i];
			location = opengl_attribute->location;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				location,
				opengl_attribute->num_floats,
				GL_FLOAT,
				GL_FALSE,
				vertex_size,
				opengl_attribute->offset);
		}
		glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_UNSIGNED_INT, NULL);
	}
	else if (opengl_model->vertex_buffer != 0) {
		//glBindVertexBuffer(GL_ARRAY_BUFFER, opengl_model->vertex_buffer, )
		//glDrawElements(GL_TRIANGLES, opengl_model->array_size, GL_FLOAT, NULL);
	}

	glBindVertexArray(0);
}

/*
 * Get number of floats that are in a given (non-matrix) variable type.
 */
static int get_num_variable_floats(const renderer_variable_type_t variable_type)
{
	// Get number of floats per attribute type.
	switch (variable_type)
	{
	case VARIABLE_FLOAT:
		return 1;
	case VARIABLE_VERTEX_2D:
		return 2;
	case VARIABLE_VERTEX_3D:
		return 3;
	case VARIABLE_VERTEX_4D:
		return 4;
	default:
		break;
	}

	return 0;
}
