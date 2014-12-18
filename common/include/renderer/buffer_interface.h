#pragma once

namespace Renderer
{

	// Interface for buffer of vertex data.
	class Buffer
	{

	public:

		// Free the buffer.
		virtual void Destroy() = 0;

	};

}