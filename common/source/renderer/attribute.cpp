#include "renderer/attribute.h"

namespace Renderer
{

	// Renderer attribute constructor.
	Attribute::Attribute(const char *name, AttributeType attributeType, DataType dataType)
		: name(name),
		attributeType(attributeType),
		dataType(dataType)
	{
	}

}