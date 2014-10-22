#include "opengl_buffer.h"

namespace OpenGL
{

	// Constructor for an empty model.
	Buffer::Buffer(const BufferSchema *schema)
		: handle(0),
		schema(schema)
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

		// Bind, load, unbind. Don't use class binds since those activate schema.
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}

	// Bind the buffer for drawing or loading data.
	void Buffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		schema->Activate();
	}

	// Unbind the buffer.
	void Buffer::Unbind()
	{
		schema->Deactivate();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
