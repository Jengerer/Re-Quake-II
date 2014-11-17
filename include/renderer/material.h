#pragma once

#include "variable.h"

namespace Renderer
{

	// Class for referencing a shader material.
	class Material
	{

	public:

		// Delete the material instance.
		virtual void Destroy() = 0;

		// Set this material as the one to render with.
		virtual void Activate() = 0;

		// Unset this material as the one to render with.
		virtual void Deactivate() = 0;

		// Prepare a schema for a buffer to map data to material attributes.
		virtual BufferSchema *CreateBufferSchema(
			const Attribute *attributes,
			int attributeCount) = 0;

		// Get a reference to a material variable.
		virtual Variable *GetVariable(const char *name) = 0;

	};

}