#pragma once

#include "buffer.h"

namespace Renderer
{

	// Schema for mapping material input attributes to a buffer.
	class BufferSchema
	{

		// Destroy the buffer schema.
		virtual void Destroy() = 0;

		// Bind a buffer to the material attributes in this schema.
		virtual void Activate(Buffer *buffer) = 0;

		// Deactivate the schema.
		virtual void Deactivate() = 0;

	};

}