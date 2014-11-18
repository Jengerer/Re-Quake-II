#pragma once

#include "jcommon_define.h"
#include "renderer/attribute.h"

namespace Renderer
{

	// Schema for mapping material input attributes to a buffer.
	class CommonLibrary BufferLayout
	{

	public:

		BufferLayout(const Attribute *attributes, int attributeCount);

		// Get an attribute by index.
		inline const Attribute *GetAttribute(int index) const
		{
			return &attributes[index];
		}

		// Get number of attributes.
		inline int GetAttributeCount() const
		{
			return attributeCount;
		}

	private:

		const Attribute *attributes;
		int attributeCount;

	};

}