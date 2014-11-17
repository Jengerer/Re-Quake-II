#include "opengl_buffer.h"

namespace OpenGL
{

	// Constructor for an empty model.
	Buffer::Buffer() : handle(0)
	{
	}

	// Destroy model and its vertex buffer.
	Buffer::~Buffer()
	{
		if (handle != 0) {
			glDeleteBuffers(1, &handle);
		}
	}

	// Initialize the buffer for a set of data.
	bool Buffer::Initialize(const void *bufferData, int bufferSize)
	{
		// Get a buffer for vertex data.
		glGenBuffers(1, &handle);
		// TODO: error checking?

		// Bind, load, unbind.
		Bind();
		glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
		Unbind();
		return true;
	}

	// Bind the buffer for drawing or loading data.
	void Buffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	// Unbind the buffer.
	void Buffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
