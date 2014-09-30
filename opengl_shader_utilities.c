#include "opengl_shader_utilities.h"
#include "error_stack.h"
#include "opengl_common.h"
#include "file.h"
#include "shared_defines.h"
#include "memory_manager.h"

// Private functions.
static void opengl_null_shader(opengl_shader_t *shader);
static void opengl_null_program(opengl_program_t *program);
static void opengl_null_shader_schema(opengl_shader_schema_t *schema);
static void opengl_null_uniform(opengl_uniform_t *uniform);
static GLenum get_opengl_shader_type(renderer_shader_type_t type);
static int get_num_variable_floats(const renderer_variable_type_t variable_type);

// Create and compile a shader from a file.
// Returns 1 on success, 0 otherwise.
// Fills out output struct as soon as something is available (so must be cleaned by caller).
int opengl_create_shader(const char *filename,
	renderer_shader_type_t type,
	renderer_shader_t *out)
{
	opengl_shader_t *shader;
	file_t shader_file; // TODO: use engine utilities for file access.
	GLuint shader_handle;
	GLchar *source;
	GLint shader_length;
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

	// Create shader handle.
	shader_type = get_opengl_shader_type(type);
	shader_handle = glCreateShader(shader_type);
	if (shader_handle == 0) {
		error_stack_log("Failed to create GL shader for '%s'.\n", filename);
		return 0;
	}
	shader->handle = shader_handle;
	
	// Load shader source and compile.
	file_null(&shader_file);
	if (file_load(filename, &shader_file) == 0) {
		error_stack_log("Failed to open shader file '%s'.\n", filename);
		file_destroy(&shader_file);
		return 0;
	}
	source = (GLchar*)shader_file.buffer;
	shader_length = (GLint)shader_file.size;
	glShaderSource(shader_handle, 1, (const GLchar**)&source, &shader_length);
	glCompileShader(shader_handle);
	file_destroy(&shader_file);

	// Print error/warning.
	glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length != 0) {
		log = (char*)memory_allocate(log_length * sizeof(GLchar));
		if (log != NULL) {
			glGetShaderInfoLog(shader_handle, log_length, &log_length, log);
			// TODO: log warnings to console.
			memory_free(log);
		}
	}

	// Check that it compiled properly.
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		error_stack_log("Failed to compile shader '%s'.\n", filename);
		return 0;
	}
	return 1;
}

// Destroy OpenGL shader.
// Takes in the program that it's attached to.
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

// Create an OpenGL shader program.
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

// Clean up OpenGL shader program.
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

// Link a shader to a program.
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

// Link and compile an OpenGL shader program.
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
			// TODO: print to console/log.
			memory_free(log);
		}
	}

	glGetProgramiv(program_handle, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		error_stack_log("Failed to link shader program.\n");
		return 0;
	}
	return 1;
}

// Set the program to be used in rendering.
void opengl_set_program(renderer_program_t program)
{
	opengl_program_t *opengl_program;
	GLuint program_handle;

	// Set program as active.
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	glUseProgram(program_handle);
}

// Unset a program from rendering.
void opengl_unset_program(void)
{
	glUseProgram(0);
}

// Generate an OpenGL shader schema for generic renderer schema.
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

// Destroy the OpenGL schema structure.
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

// Return a handle to an OpenGL shader uniform variable.
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
		error_stack_log("Failed to allocate uniform structure.\n");
		return 0;
	}
	opengl_null_uniform(opengl_uniform);
	out->buffer = opengl_uniform;

	// Find the variable location.
	opengl_program = (opengl_program_t*)program.buffer;
	program_handle = opengl_program->handle;
	location = glGetUniformLocation(program_handle, (const GLchar*)name);
	if (location == -1) {
		error_stack_log("Failed to get location for uniform variable '%s'.\n", name);
		return 0;
	}
	opengl_uniform->location = location;
	return 1;
}

// Free a handle to a uniform variable.
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

// Set the value of a 3D vector uniform variable.
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

// Set the value of a 4D vector uniform variable.
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

// Set the value of a 3x3 matrix uniform variable.
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

// Set the value of a 4x4 matrix uniform variable.
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

// Set the value of an integer uniform variable.
void opengl_set_uniform_integer(
	renderer_uniform_t uniform,
	int value)
{
	opengl_uniform_t *opengl_uniform;
	GLint location;

	// Set the value.
	opengl_uniform = (opengl_uniform_t*)uniform.buffer;
	location = opengl_uniform->location;
	glUniform1i(location, value);
}

/* Null OpenGL shader to uninitialized state. */
void opengl_null_shader(opengl_shader_t *shader)
{
	shader->handle = 0;
	shader->is_linked = 0;
}

// Null OpenGL program.
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

// Null OpenGL uniform variable structure.
void opengl_null_uniform(opengl_uniform_t *uniform)
{
	uniform->location = -1;
}

// Convert renderer shader type to OpenGL type.
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
	error_stack_log("Invalid shader type found!");
	return 0;
}

// Get number of floats that are in a given (non-matrix) variable type.
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
