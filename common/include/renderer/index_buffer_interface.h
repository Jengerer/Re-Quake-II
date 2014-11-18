#pragma once

#include "renderer/shared.h"

namespace Renderer
{

	// Buffer that references index data.
	class IndexBuffer
	{

	public:

		// Free the index buffer.
		virtual void Destroy() = 0;

		// Set the index data.
		virtual void Load(
			const void *indices,
			unsigned int bufferSize,
			unsigned int indexCount,
			Renderer::DataType indexType) = 0;

		// Activate the index buffer for rendering.
		virtual void Activate() = 0;

		// Deactivate the index buffer from rendering.
		virtual void Deactivate() = 0;

		// Send a draw call using this index buffer.
		virtual void Draw(PrimitiveType primitiveType) = 0;

	};

}