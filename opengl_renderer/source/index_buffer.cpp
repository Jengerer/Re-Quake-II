#include "error_stack.h"
#include "index_buffer.h"
#include "memory_manager.h"

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
		MemoryManager::Destroy(this);
	}

	// Initialize from index data.
	void IndexBuffer::Load
		(const void *indices,
		unsigned int bufferSize,
		unsigned int indexCount,
		Renderer::DataType indexType)
	{
		// Set new index count and type.
		this->count = indexCount;
		this->type = TranslateIndexType(indexType);

		// Pass new data to buffer.
		Activate();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices, GL_STATIC_DRAW);
		Deactivate();
	}

	// Bind indices as the element reference.
	void IndexBuffer::Activate()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	// Unbind indices from element reference.
	void IndexBuffer::Deactivate()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Make a draw call using this element indices.
	// Assumes buffer was previously bound.
	void IndexBuffer::Draw(Renderer::PrimitiveType type)
	{
		GLenum primitive = Common::TranslatePrimitiveType(type);
		glDrawElements(primitive, count, type, nullptr);
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