#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include "opengl_buffer.h"
#include "opengl_buffer_schema.h"
#include "opengl_index_buffer.h"
#include "opengl_program.h"
#include "opengl_resources.h"
#include "opengl_shader.h"
#include "opengl_uniform.h"
#include "shared_defines.h"

namespace OpenGL
{

	// Singleton instance instantiation.
	Resources Resources::instance;

	// Create shader from file.
	Renderer::Shader *Resources::CreateShader(const char *filename, Renderer::ShaderType type)
	{
		// Allocate a shader object.
		Shader *glShader;
		if (!MemoryManager::Allocate(&glShader)) {
			ErrorStack::Log("Failed to allocate OpenGL shader object.\n");
			return false;
		}
		new (glShader) Shader();

		// Initialize shader from source.
		if (glShader->Initialize(filename, type)) {
			return static_cast<Renderer::Shader*>(glShader);
		}

		// Initialization failed, clean up.
		MemoryManager::Destroy(glShader);
		return nullptr;
	}

	// Destroy shader.
	// Shader is automatically unlinked.
	void Resources::DestroyShader(Renderer::Shader *shader)
	{
		Shader *glShader = static_cast<Shader*>(shader);
		MemoryManager::Destroy(glShader);
	}

	// Create shader program from vertex/fragment shaders.
	Renderer::Program *Resources::CreateProgram(const Renderer::Shader *vertex, const Renderer::Shader *fragment)
	{
		// Allocate program object.
		Program *program;
		if (!MemoryManager::Allocate(&program)) {
			ErrorStack::Log("Failed to allocate OpenGL shader program.\n", program);
			return nullptr;
		}
		new (program) Program();

		// Initialize with shaders.
		if (program->Initialize(vertex, fragment)) {
			return static_cast<Renderer::Program*>(program);
		}

		// Failed, clean up.
		MemoryManager::Destroy(program);
		return nullptr;
	}

	// Destroy program.
	void Resources::DestroyProgram(Renderer::Program *program)
	{
		Program *glProgram = static_cast<Program*>(program);
		MemoryManager::Destroy(glProgram);
	}

	// Convert generic renderer schema to OpenGL schema.
	Renderer::ShaderSchema *Resources::CreateBufferSchema(
		const Renderer::Program *program,
		const Renderer::Attribute *attributes,
		int attributeCount)
	{
		// Allocate space for the schema.
		BufferSchema *schema;
		if (!MemoryManager::Allocate(&schema)) {
			ErrorStack::Log("Failed to allocate OpenGL buffer schema object.\n");
			return nullptr;
		}

		// Initialize from program and attribute data.
		const Program *glProgram = static_cast<const Program*>(program);
		if (schema->Initialize(glProgram, attributes, attributeCount)) {
			return schema;
		}

		// Failed, clean up.
		MemoryManager::Destroy(schema);
		return nullptr;
	}

	// Clean up buffer schema.
	void Resources::DestroyBufferSchema(Renderer::BufferSchema *schema)
	{
		BufferSchema *glSchema = static_cast<BufferSchema*>(schema);
		MemoryManager::Destroy(glSchema);
	}

	// Generate a buffer from a set of data.
	Renderer::Buffer *Resources::CreateBuffer(
		const void *bufferData,
		int bufferSize,
		const Renderer::BufferSchema *schema)
	{
		// Allocate space for the OpenGL object.
		Buffer *buffer;
		if (!MemoryManager::Allocate(&buffer)) {
			ErrorStack::Log("Failed to allocate OpenGL buffer object.");
			return nullptr;
		}
		new (buffer) Buffer(schema);

		// Initialize the buffer.
		if (!buffer->Initialize(bufferData, bufferSize)) {
			MemoryManager::Destroy(buffer);
			return nullptr;
		}
		return buffer;
	}

	// Destroy the buffer object.
	void Resources::DestroyBuffer(Renderer::Buffer *buffer)
	{
		if (buffer != nullptr) {
			OpenGL::Buffer *glBuffer = static_cast<OpenGL::Buffer*>(buffer);
			MemoryManager::Destroy(glBuffer);
		}
	}

	// Create an index buffer to reference vertex data.
	Renderer::IndexBuffer *Resources::CreateIndexBuffer(
		const void *indices,
		int bufferSize,
		int indexCount,
		Renderer::DataType type)
	{
		// Allocate space for the OpenGL object.
		IndexBuffer *buffer;
		if (!MemoryManager::Allocate(&buffer)) {
			ErrorStack::Log("Failed to allocate OpenGL index buffer object.");
			return nullptr;
		}
		new (buffer) IndexBuffer(type, indexCount)

		// Initialize the buffer.
		if (!buffer->Initialize(indices, bufferSize)) {
			MemoryManager::Destroy(buffer);
			return nullptr;
		}
		return buffer;
	}

	// Destroy an index buffer.
	void Resources::DestroyIndexBuffer(Renderer::IndexBuffer *buffer) {
		if (buffer != nullptr) {
			IndexBuffer glBuffer = static_cast<IndexBuffer*>(buffer);
			MemoryManager::Destroy(glBuffer);
		}
	}

	// Get a uniform from a program.
	// Program must be bound before this is called.
	Renderer::Uniform *Resources::GetUniform(const Renderer::Program *program, const char *name)
	{
		// Get uniform location.
		const Program *glProgram = static_cast<const Program*>(program);
		GLint location = glProgram->GetUniformLocation(name);
		if (location == -1) {
			ErrorStack::Log("Failed to get location for uniform variable: %s", name);
			return nullptr;
		}

		// Allocate uniform object.
		Uniform *uniform;
		if (!MemoryManager::Allocate(&uniform)) {
			ErrorStack::Log("Failed to allocate OpenGL uniform object.\n");
			return nullptr;
		}
		new (uniform) Uniform(location);
		return uniform;
	}

	// Destroy a uniform variable.
	void Resources::DestroyUniform(Renderer::Uniform *uniform)
	{
		Uniform *glUniform = static_cast<Uniform*>(uniform);
		MemoryManager::Destroy(glUniform);
	}

	/*
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
	*/

	// Get singleton instance of resource loader.
	Resources *Resources::GetInstance()
	{
		return &instance;
	}

};
