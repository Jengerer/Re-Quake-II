#include "opengl_renderer.h"
#include "opengl_shader_utilities.h"
#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"

// Singleton instance.
opengl_context_t opengl;

// Private functions.
static void opengl_null_texture(opengl_texture_t *texture);
static void opengl_null_model(opengl_model_t *model);

// Initialize OpenGL scene and context.
int opengl_initialize(void)
{
	// Initialize GLEW.
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		error_stack_log("Unable to initialize GLEW: %s.\n", glewGetErrorString(error));
		return 0;
	}

	// Enable vertical sync (TODO: make this an option).

	// Set up GL rendering parameters.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return 1;
}

// Deallocate OpenGL context.
void opengl_destroy(void)
{
}

// Create 2D texture from image.
int opengl_create_texture2d(
	const image_t *image,
	renderer_texture_t *out)
{
	opengl_texture_t *opengl_texture;
	GLuint texture_handle;

	// Allocate space for the structure.
	opengl_texture = (opengl_texture_t*)memory_allocate(sizeof(opengl_model_t));
	if (opengl_texture == NULL) {
		return 0;
	}
	opengl_null_texture(opengl_texture);
	out->buffer = opengl_texture;

	// Get a texture handle.
	glGenTextures(1, &texture_handle);
	opengl_texture->handle = texture_handle;

	// Bind the texture and load the image data.
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->buffer);

	// Don't tile the textures.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear interpolation for smaller and larger textures.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	return 1;
}

// Bind a 2D texture to render.
void opengl_bind_texture2d(renderer_texture_t texture, renderer_uniform_t shader_texture)
{
	// Bind the texture.
	opengl_texture_t *opengl_texture = (opengl_texture_t*)texture.buffer;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, opengl_texture->handle);

	// Set the uniform value to match this texture slot.
	// TODO: Clean this up to properly use texture indices.
	opengl_set_uniform_integer(shader_texture, 0);
}

// Unbind the 2D texture.
void opengl_unbind_texture2d(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Create a renderer context for a given mesh.
// Returns 1 and fills out struct on success, returns 0 otherwise.
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
	model->array_size = num_GLvertices;
	return 1;
}

// Create a renderer context for a given indexed mesh.
// Returns 1 and fills out struct on success, returns 0 otherwise.
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

// Destroy the OpenGL model.
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

// Clear an OpenGL scene for a new frame.
void opengl_clear_scene(void)
{
	// TODO: If we're completely drawing over a scene, we don't need this.
	glClear(GL_COLOR_BUFFER_BIT);
}

// Render an OpenGL model.
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
}

// Null OpenGL texture.
void opengl_null_texture(opengl_texture_t *texture)
{
	texture->handle = 0;
}

// Null OpenGL model.
void opengl_null_model(opengl_model_t *model)
{
	model->array_size = 0;
	model->index_buffer = 0;
	model->vertex_buffer = 0;
}