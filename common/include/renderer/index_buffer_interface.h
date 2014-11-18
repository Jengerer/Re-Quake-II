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

	};

}