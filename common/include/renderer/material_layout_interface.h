#pragma once

#include "renderer/buffer_interface.h"
#include "renderer/buffer_layout.h"

namespace Renderer
{

	// Class for managing how multiple buffers map to material attributes.
	class MaterialLayout
	{

	public:

		// Destroy this layout reference.
		virtual void Destroy() = 0;

		// Bind a buffer to a specific layout.
		virtual void BindBuffer(int bufferIndex, const Buffer *buffer) = 0;

	};

}