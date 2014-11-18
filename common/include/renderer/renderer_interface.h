#pragma once

#include "shared.h"
#include "renderer/material_interface.h"
#include "renderer/material_layout_interface.h"

namespace Renderer
{

	// Renderer drawing interface.
	class Interface
	{

	public:

		// Initialize renderer.
		virtual bool Initialize() = 0;

		// Shut down renderer.
		virtual void Destroy() = 0;

		// Clear the scene for a new frame.
		virtual void ClearScene() = 0;

		// Set the material to render with.
		virtual void SetMaterial(Renderer::Material *material) = 0;

		// Unset a material from rendering.
		virtual void UnsetMaterial(Renderer::Material *material) = 0;

		// Activate a material layout.
		virtual void SetMaterialLayout(MaterialLayout *layout) = 0;

		// Deactivate a material layout.
		virtual void UnsetMaterialLayout(MaterialLayout *layout) = 0;

	};

}