#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "renderer/buffer_interface.h"
#include "renderer/index_buffer_interface.h"
#include "renderer/material_interface.h"
#include "renderer/shared.h"
#include "vector3.h"
#include "vector4.h"

namespace Renderer
{

	// Interface for preparing shaders and uniform variables.
	class Resources
	{

	public:

		// Load a material defined by a vertex/pixel shader.
		virtual Material *CreateMaterial(const char *name, const Renderer::MaterialLayout *layout) = 0;

		// Generate a buffer for vertex data.
		virtual Buffer *CreateBuffer() = 0;

		// Generate an index buffer for referencing vertex data.
		virtual IndexBuffer *CreateIndexBuffer() = 0;

	};

}
