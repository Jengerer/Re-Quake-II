#pragma once

namespace Renderer
{

	// Buffer of vertex data.
	class Buffer
	{

	public:

		// Free the buffer.
		virtual void Destroy() = 0;

		// Set the data for the buffer.
		virtual void Load(const void *data, unsigned int size) = 0;

	};

}