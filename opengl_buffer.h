#pragma once

#include "opengl_common.h"
#include "opengl_buffer_schema.h"

namespace OpenGL
{

	// Class representing a buffer of data passed to shader.
	class Buffer : public Renderer::Buffer
	{

	public:

		Buffer(const BufferSchema *schema);
		~Buffer();

		// Create the model from a set of vertex data.
		bool Initialize(const void *bufferData,	int bufferSize);

		// Bind the buffer for rendering.
		void Bind();

		// Unbind the buffer from rendering.
		void Unbind();

	private:

		GLuint handle;
		const BufferSchema *schema;

	};

}
