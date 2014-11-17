#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Class representing an index buffer.
	class IndexBuffer : public Renderer::Buffer
	{

	public:

		IndexBuffer(Renderer::DataType type, int indexCount);
		~IndexBuffer();

		// Initialize from a set of index data.
		bool Initialize(const void *data, int bufferSize);

		// Bind the index buffer to be used for element data.
		void Bind();

		// Unbind the buffer from rendering.
		void Unbind();

		// Make a draw call for this index data.
		// Assumes buffer was previously bound.
		void Draw(GLenum geometryType);

	private:

		// Convert data type to OpenGL enum.
		static GLenum TranslateIndexType(Renderer::DataType indexType);

	private:

		GLuint handle;
		GLenum type;
		GLint count;

	};

}
