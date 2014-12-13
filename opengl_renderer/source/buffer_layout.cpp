#include "buffer_layout.h"
#include <error_stack.h>
#include <memory_manager.h>

namespace OpenGL
{

	// Shader schema constructor.
	BufferLayout::BufferLayout() : attributes(nullptr), attributeCount(0), stride(0)
	{
	}

	// Destroy shader schema and its attributes.
	BufferLayout::~BufferLayout()
	{
		// Destroy attributes if there are any.
		if (attributes != nullptr) {
			delete[] attributes;
		}
	}

	// Set the attribute array for this layout.
	void BufferLayout::SetAttributes(Attribute *attributes, int attributeCount)
	{
		this->attributes = attributes;
		this->attributeCount = attributeCount;
	}

	// Set up the vertex attributes for this layout.
	void BufferLayout::Activate() const
	{
		// Activate each vertex attribute.
		const Attribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Activate(stride);
		}
	}

	// Unset the attributes from rendering.
	void BufferLayout::Deactivate() const
	{
		// Deactivate each vertex attribute.
		const Attribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Deactivate();
		}
	}

}