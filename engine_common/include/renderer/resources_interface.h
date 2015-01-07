#pragma once

#include "renderer/buffer_interface.h"
#include "renderer/index_buffer_interface.h"
#include "renderer/material_interface.h"
#include "renderer/shared.h"
#include "renderer/texture_interface.h"
#include <image.h>

namespace Renderer
{

	// Interface for preparing shaders and uniform variables.
	class Resources
	{

	public:

		// Load a material defined by a vertex/pixel shader.
		virtual Material *CreateMaterial(const char *vertexFile, const char *pixelFile) = 0;

		// Generate a buffer for vertex data.
		virtual Buffer *CreateBuffer(
			const void *data,
			unsigned int bufferSize) = 0;

		// Generate an index buffer for referencing vertex data.
		virtual IndexBuffer *CreateIndexBuffer(
			const void *indices,
			unsigned int bufferSize,
			Renderer::DataType indexType) = 0; 

		// Create texture from image data.
		virtual Texture *CreateTexture(const Image<PixelRGBA> *image) = 0;

	};

}
