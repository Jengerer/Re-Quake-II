#pragma once

#include "common.h"
#include "renderer/buffer_interface.h"

namespace OpenGL
{

	// Class representing a buffer of data passed to shader.
	class Buffer : public Renderer::Buffer
	{

	public:

		Buffer();

		// Initialize the buffer for loading.
		bool Initialize();

		// Free the buffer.
		virtual void Destroy();

		// Set the buffer data.
		bool Load(const void *data, unsigned int size);

		// Bind the buffer for rendering.
		void Bind() const;

		// Unbind the buffer from rendering.
		void Unbind() const;

	private:

		// Private destructor, must be killed through Destroy().
		~Buffer();

	private:

		GLuint handle;

	};

}
