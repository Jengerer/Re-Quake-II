#include "buffer.h"
#include "index_buffer.h"
#include "material.h"
#include "resources.h"
#include "texture.h"
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
            delete material;
            ErrorStack::Log("Failed to initialize material for files %s and %s.", vertexFile, pixelFile);
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
            delete buffer;
			return nullptr;
		}
		if (!buffer->Load(data, bufferSize)) {
            delete buffer;
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
            delete buffer;
			return nullptr;
		}
		if (!buffer->Load(indices, bufferSize, indexType)) {
            delete buffer;
			return nullptr;
		}
		return buffer;
	}

	// Create a texture from a set of image data.
	Renderer::Texture *Resources::CreateTexture(const Image<PixelRGBA> *image)
	{
		Texture *texture = new Texture();
		if (texture == nullptr) {
			ErrorStack::Log("Failed to allocate OpenGL texture object.");
			return nullptr;
		}
		if (!texture->Initialize()) {
            delete texture;
			ErrorStack::Log("Failed to initialize OpenGL texture object.");
			return nullptr;
		}
		if (!texture->Load(image)) {
            delete texture;
			return nullptr;
		}
		return static_cast<Renderer::Texture*>(texture);
	}

	// Get singleton instance of resource loader.
	Resources *Resources::GetInstance()
	{
		return &instance;
	}

};
