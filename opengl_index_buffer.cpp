#include "error_stack.h"
#include "opengl_index_buffer.h"

namespace OpenGL
{

	IndexBuffer::IndexBuffer(Renderer::DataType type, int indexCount)
		: handle(0),
		count(indexCount)
	{
		// Translate the type.
		this->type = TranslateIndexType(type);
	}

	// Release OpenGL buffer.
	IndexBuffer::~IndexBuffer()
	{
		if (handle != 0) {
			glDeleteBuffers(1, &handle);
		}
	}

	// Initialize from index data.
	bool IndexBuffer::Initialize(const void *indices, int bufferSize)
	{
		// Generate a buffer.	
		// TODO: Error checking?
		glGenBuffers(1, &handle);

		// Bind, load, unbind.
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices, GL_STATIC_DRAW);
		Unbind();
		return true;
	}

	// Bind indices as the element reference.
	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	// Unbind indices from element reference.
	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Make a draw call using this element indices.
	// Assumes buffer was previously bound.
	void IndexBuffer::Draw(GLenum geometryType)
	{
		glDrawElements(geometryType, count, type, nullptr);
	}

	// Translate index type to OpenGL type.
	// Assumes data type is an integral type.
	GLenum IndexBuffer::TranslateIndexType(Renderer::DataType indexType)
	{
		switch (indexType) {
		case Renderer::ByteType:
			return GL_BYTE;
		case Renderer::UnsignedByteType:
			return GL_UNSIGNED_BYTE;
		case Renderer::ShortType:
			return GL_SHORT;
		case Renderer::UnsignedShortType:
			return GL_UNSIGNED_SHORT;
		case Renderer::IntType:
			return GL_INT;
		case Renderer::UnsignedIntType:
			return GL_UNSIGNED_INT;
		default:
			ErrorStack::Log("Expected integral type for index data type.");
			return 0;
		}
	}

}