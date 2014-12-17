#include "buffer.h"
#include "index_buffer.h"
#include "material.h"
#include "resources.h"
#include <error_stack.h>
#include <file.h>
#include <memory_manager.h>

namespace OpenGL
{

	// Singleton instance instantiation.
	Resources Resources::instance;

	// Create material by vertex/pixel shader.
	Renderer::Material *Resources::CreateMaterial(const char *vertexFile, const char *pixelFile)
	{
		FileData vertexData, pixelData;
		File shaderFile;
		if (!shaderFile.Open(vertexFile, File::ReadMode)) {
			ErrorStack::Log("Failed to open vertex shader: %s.", vertexFile);
			return nullptr;
		}
		if (!shaderFile.Read(&vertexData)) {
			ErrorStack::Log("Failed to read vertex shader from file: %s.", vertexFile);
			return nullptr;
		}
		if (!shaderFile.Open(pixelFile, File::ReadMode)) {
			ErrorStack::Log("Failed to open pixel shader file: %s.", pixelFile);
			return nullptr;
		}
		if (!shaderFile.Read(&pixelData)) {
			ErrorStack::Log("Failed to read pixel shader from file: %s.", pixelFile);
			return nullptr;
		}

		// Create material object.
		Material *material = new Material();
		if (material == nullptr) {
			ErrorStack::Log("Failed to allocate renderer material object.");
			return nullptr;
		}

		// Build with the source.
		const char *vertexSource = reinterpret_cast<const char*>(vertexData.GetData());
		const char *pixelSource = reinterpret_cast<const char*>(pixelData.GetData());
		int32_t vertexLength = vertexData.GetSize();
		int32_t pixelLength = pixelData.GetSize();
		if (!material->Initialize(vertexSource, vertexLength, pixelSource, pixelLength)) {
			material->Destroy();
			return nullptr;
		}
		return material;
	}

	// Generate a buffer from a set of data.
	Renderer::Buffer *Resources::CreateBuffer(
		const void *data,
		unsigned int bufferSize)
	{
		// Allocate space for the OpenGL object.
		Buffer *buffer = new Buffer();
		if (buffer == nullptr) {
			ErrorStack::Log("Failed to allocate OpenGL buffer object.");
			return nullptr;
		}

		// Initialize the buffer.
		if (!buffer->Initialize()) {
			buffer->Destroy();
			return nullptr;
		}
		if (!buffer->Load(data, bufferSize)) {
			buffer->Destroy();
			return nullptr;
		}
		return buffer;
	}

	// Create an index buffer to reference vertex data.
	Renderer::IndexBuffer *Resources::CreateIndexBuffer(
		const void *indices,
		unsigned int bufferSize,
		Renderer::DataType indexType)
	{
		// Allocate space for the OpenGL object.
		IndexBuffer *buffer = new IndexBuffer();
		if (buffer == nullptr) {
			ErrorStack::Log("Failed to allocate OpenGL index buffer object.");
			return nullptr;
		}

		// Initialize the buffer.
		if (!buffer->Initialize()) {
			buffer->Destroy();
			return nullptr;
		}
		if (!buffer->Load(indices, bufferSize, indexType)) {
			buffer->Destroy();
			return nullptr;
		}
		return buffer;
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
