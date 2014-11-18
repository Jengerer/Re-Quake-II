#pragma once

#include "allocatable.h"
#include "attribute.h"
#include "common.h"

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

		// Enable this schema for the current buffer.
		void Activate() const;

		// Disable this schema from rendering.
		void Deactivate() const;

	private:

		Attribute *attributes;
		int attributeCount;

	};

}