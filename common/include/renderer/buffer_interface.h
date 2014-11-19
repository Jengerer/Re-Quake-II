#pragma once

namespace Renderer
{

	// Buffer of vertex data.
	class Buffer
	{

	public:

		// Free the buffer.
		virtual void Destroy() = 0;

	};

}