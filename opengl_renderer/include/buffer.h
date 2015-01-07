#pragma once

#include "common.h"
#include <allocatable.h>
#include <renderer/buffer_interface.h>

namespace OpenGL
{

	// Class representing a buffer of data passed to shader.
	class Buffer : public Renderer::Buffer, public Allocatable
	{

	public:

		Buffer();
        virtual ~Buffer();

		// Initialize the buffer for loading.
		bool Initialize();

		// Set the buffer data.
		bool Load(const void *data, unsigned int size);

		// Bind the buffer for rendering.
		void Bind() const;

		// Unbind the buffer from rendering.
		void Unbind() const;

	private:

		GLuint handle;

	};

}
