#include "opengl_index_buffer.h"

IndexBuffer::IndexBuffer(DataType type, int indexCount)
	: count(indexCount)
{
	// Translate the type.
	this->type = TranslateIndexType(type);
}

// Release OpenGL buffer.
~IndexBuffer::IndexBuffer()
{
	if (handle != 0) {
		glDeleteBuffers(1, &handle);
	}
}

// Initialize from index data.
bool IndexBuffer::Initialize(const void *data, int bufferSize)
{
	// Generate a buffer.	
	// TODO: Error checking?
	glGenBuffers(1, &handle);

	// Bind, load, unbind.
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
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
void IndexBuffer::Draw()
{
	glDrawElements(type, count, nullptr);
}

// Translate index type to OpenGL type.
// Assumes data type is an integral type.
GLenum TranslateIndexType(DataType indexType)
{
	switch (indexType) {
	case Unsigned
}
