#include "index_buffer.h"
#include <error_stack.h>
#include <memory_manager.h>

namespace OpenGL
{

	IndexBuffer::IndexBuffer()
	{
	}

	// Initialize the index buffer.
	bool IndexBuffer::Initialize()
	{
		glGenBuffers(1, &handle);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to generate OpenGL index buffer.");
			return false;
		}
		return true;
	}

	// Destroy this index buffer.
	void IndexBuffer::Destroy()
	{
		delete this;
	}

	// Initialize from index data.
	bool IndexBuffer::Load(
		const void *indices,
		unsigned int bufferSize,
		Renderer::DataType indexType)
	{
		// Set new index count and type.
		this->type = TranslateIndexType(indexType);

		// Pass new data to buffer.
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices, GL_STATIC_DRAW);
		if (glGetError() != GL_NO_ERROR) {
			Unbind();
			ErrorStack::Log("Failed to load %u bytes of data to buffer.", bufferSize);
			return false;
		}
		Unbind();
		return true;
	}

	// Bind indices as the element reference.
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	// Unbind indices from element reference.
	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Release OpenGL buffer.
	IndexBuffer::~IndexBuffer()
	{
		if (handle != 0) {
			glDeleteBuffers(1, &handle);
		}
	}

	// Translate index type to OpenGL type.
	// Assumes data type is an integral type.
	GLenum IndexBuffer::TranslateIndexType(Renderer::DataType indexType)
	{
		switch (indexType) {
		case Renderer::UnsignedByteType:
			return GL_UNSIGNED_BYTE;
		case Renderer::UnsignedShortType:
			return GL_UNSIGNED_SHORT;
		case Renderer::UnsignedIntType:
			return GL_UNSIGNED_INT;
		default:
			ErrorStack::Log("Expected unsigned integral type for index data type.");
			return 0;
		}
	}

}