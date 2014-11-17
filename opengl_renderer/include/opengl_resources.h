#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "common.h"
#include "renderer/resources.h"
#include "vector3.h"
#include "vector4.h"

namespace OpenGL
{

	// Implementing class for loading renderer resources.
	class Resources : public Renderer::Resources
	{

	public:

		// Load a material defined by a vertex/pixel shader.
		virtual Renderer::Material *CreateMaterial(const char *name, const Renderer::MaterialLayout *layout);

		// Generate a buffer for vertex data.
		virtual Renderer::Buffer *CreateBuffer();

		// Generate an index buffer for referencing vertex data.
		virtual Renderer::IndexBuffer *CreateIndexBuffer();

	public:

		// Singleton getter.
		static Resources *GetInstance();

	private:

		// Singleton instance.
		static Resources instance;

	};

}
