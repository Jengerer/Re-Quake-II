#pragma once

namespace Renderer
{

	// Buffer that references index data.
	class IndexBuffer
	{

	public:

		// Free the index buffer.
		virtual void Destroy() = 0;

		// Set the index data.
		virtual void Load(const unsigned int *indices, unsigned int indexCount) = 0;

		// Activate the index buffer for rendering.
		virtual void Activate() = 0;

		// Deactivate the index buffer from rendering.
		virtual void Deactivate() = 0;

		// Send a draw call using this index buffer.
		virtual void Draw() = 0;

	};

}