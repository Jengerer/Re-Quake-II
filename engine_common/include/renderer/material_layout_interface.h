#pragma once

#include "renderer/buffer_interface.h"
#include "renderer/buffer_layout.h"

namespace Renderer
{

	// Class for managing how multiple buffers map to material attributes.
	class MaterialLayout
	{

	public:

        // Layout virtual destructor.
        virtual ~MaterialLayout() = default;

		// Bind a buffer to a specific layout.
		virtual void BindBuffer(int bufferIndex, const Buffer *buffer) = 0;

	};

}
