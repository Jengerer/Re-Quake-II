#include "buffer.h"
#include <error_stack.h>
#include <memory_manager.h>

namespace OpenGL
{

	Buffer::Buffer() : handle(0)
	{
	}

	Buffer::~Buffer()
	{
		if (handle != 0) {
			glDeleteBuffers(1, &handle);
		}
	}

	// Create the buffer instance.
	bool Buffer::Initialize()
	{
		// Get a buffer for vertex data.
		glGenBuffers(1, &handle);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to allocate vertex buffer.");
			return false;
		}
		return true;
	}

	// Load data into the buffer.
	bool Buffer::Load(const void *data, unsigned int size)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		if (glGetError() != GL_NO_ERROR) {
			Unbind();
			ErrorStack::Log("Failed to load %u bytes of data to buffer.", size);
			return false;
		}
		Unbind();
		return true;
	}

	// Bind the buffer for drawing or loading data.
	void Buffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	// Unbind the buffer.
	void Buffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
