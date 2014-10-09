#include "opengl_buffer.h"

namespace OpenGL
{

	// Constructor for an empty model.
	Buffer::Buffer() : vertexBuffer(0)
	{
	}

	// Destroy model and its vertex buffer.
	Buffer::~Buffer()
	{
		if (vertexBuffer != 0) {
			glDeleteBuffers(1, &vertexBuffer);
		}
	}

	// Initialize the buffer for a set of data.
	bool Buffer::Initialize(const void *bufferData, int bufferSize)
	{
		// Get a buffer for vertex data.
		glGenBuffers(1, &vertexBuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	}

	// Unbind the buffer.
	void Buffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
