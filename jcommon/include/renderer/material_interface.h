#pragma once

#include "renderer/material_layout_interface.h"
#include "renderer/variable_interface.h"

namespace Renderer
{

	// Class for referencing a shader material.
	class Material
	{

	public:

		// Delete the material instance.
		virtual void Destroy() = 0;

		// Get a reference to a material variable.
		virtual Variable *GetVariable(const char *name) = 0;

		// Get a matching material layout for this material.
		virtual MaterialLayout *GetLayout(
			const BufferLayout *bufferLayouts,
			int bufferCount) = 0;

		// Set this material as the one to render with.
		// This function must be called after all the buffers are bound.
		virtual void Activate() = 0;

		// Unset this material as the one to render with.
		virtual void Deactivate() = 0;

	};

}