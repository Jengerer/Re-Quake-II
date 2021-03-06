#pragma once

#include "common.h"
#include <matrix3x3.h>
#include <matrix4x4.h>
#include <renderer/resources_interface.h>
#include <vector3.h>
#include <vector4.h>

namespace OpenGL
{

	// Implementing class for loading renderer resources.
	class Resources : public Renderer::Resources
	{

	public:

		// Load a material defined by a vertex/pixel shader.
		virtual Renderer::Material *CreateMaterial(const char *vertexFile, const char *pixelFile);

		// Generate a buffer for vertex data.
		virtual Renderer::Buffer *CreateBuffer(
			const void *data,
			unsigned int bufferSize);

		// Generate an index buffer for referencing vertex data.
		virtual Renderer::IndexBuffer *CreateIndexBuffer(
			const void *indices,
			unsigned int bufferSize,
			Renderer::DataType indexType);

		// Create a texture from an image.
		virtual Renderer::Texture *CreateTexture(const Image<PixelRGBA> *image);

	public:

		// Singleton getter.
		static Resources *GetInstance();

	private:

		// Singleton instance.
		static Resources instance;

	};

}
