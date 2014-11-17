#pragma once

#include "renderer/shared.h"

namespace Renderer
{

	// Single input attribute for a material.
	// Intended to be used as a static constant object (does not copy the string).
	class Attribute
	{

	public:

		Attribute(const char *name, AttributeType attributeType, DataType dataType);

		// Get the variable name for this attribute.
		inline const char *GetName() const
		{
			return name;
		}

		// Get the semantic type for this attribute.
		inline AttributeType GetAttributeType() const
		{
			return attributeType;
		}

		// Get the data storage type for this attribute.
		inline DataType GetDataType() const
		{
			return dataType;
		}

	private:

		const char *name;
		AttributeType attributeType;
		DataType dataType;

	};

}