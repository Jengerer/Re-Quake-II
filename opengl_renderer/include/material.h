#pragma once

#include "common.h"
#include <allocatable.h>
#include <renderer/material_interface.h>

namespace OpenGL
{

	// Full OpenGL shader program.
	class Material : public Renderer::Material, public Allocatable
	{

	public:

		Material();
        virtual ~Material();

		// Initialize the material.
		bool Initialize(
			const char *vertexSource,
			int vertexSourceLength,
			const char *pixelSource,
			int pixelSourceLength);

		// Get a reference to a variable in this material.
		virtual Renderer::Variable *GetVariable(const char *name);

		// Get a matching material layout for this material.
		virtual Renderer::MaterialLayout *GetLayout(
			const Renderer::BufferLayout *bufferLayouts,
			int bufferCount);

		// Set this program as active.
		void Activate();

		// Unset this program from rendering.
		void Deactivate();

	private:

		// Compile a shader from source.
		static bool Compile(GLuint shader, const char *source, int sourceLength);

	private:

		GLuint handle;
		GLuint vertexShader;
		GLuint pixelShader;

	};

}
