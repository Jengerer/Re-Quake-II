#pragma once

#include "attribute.h"
#include "common.h"
#include <allocatable.h>

namespace OpenGL
{

	// Structure for representing an OpenGL buffer layout.
	class BufferLayout : public Allocatable
	{

	public:

		BufferLayout();
		~BufferLayout();

		// Set the attributes array for this layout.
		void SetAttributes(Attribute *attributes, int attributeCount);

		// Set the total size of the attributes.
		inline void SetStride(int stride) { this->stride = stride; }

		// Enable this schema for the current buffer.
		void Activate() const;

		// Disable this schema from rendering.
		void Deactivate() const;

	private:

		Attribute *attributes;
		int attributeCount;
		int stride;

	};

}