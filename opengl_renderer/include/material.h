#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// OpenGL full shader program object.
	class Program : public Renderer::Program
	{

	public:

		Program();
		~Program();

		// Initialize program from vertex and fragment shaders.
		bool Initialize(const Renderer::Shader *vertex, const Renderer::Shader *fragment);

		// Set this program as active.
		void Activate() const;

		// Unset this program from rendering.
		void Deactivate() const;

		// Get location of a uniform within this program.
		GLint GetUniformLocation(const char *name) const;

	private:

		GLuint handle;

	};

}