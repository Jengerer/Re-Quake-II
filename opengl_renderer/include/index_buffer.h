#pragma once

#include "allocatable.h"
#include "common.h"
#include "renderer/index_buffer_interface.h"

namespace OpenGL
{

	// Class representing an index buffer.
	class IndexBuffer : public Renderer::IndexBuffer, public Allocatable
	{

	public:

		IndexBuffer();

		// Initialize the index buffer.
		bool Initialize();

		// Destroy this index buffer.
		virtual void Destroy();

		// Initialize from a set of index data.
		void Load(
			const void *indices,
			unsigned int bufferSize,
			unsigned int indexCount,
			Renderer::DataType indexType);

		// Bind the index buffer to be used for element data.
		virtual void Activate();

		// Unbind the buffer from rendering.
		virtual void Deactivate();

		// Make a draw call for this index data.
		// Assumes buffer was previously bound.
		virtual void Draw(Renderer::PrimitiveType type);

	private:

		// Private destructor; must be killed through Destroy().
		~IndexBuffer();

	private:

		// Convert data type to OpenGL enum.
		static GLenum TranslateIndexType(Renderer::DataType indexType);

	private:

		GLuint handle;
		GLenum type;
		GLint count;

	};

}
