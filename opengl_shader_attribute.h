#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Object for representing an OpenGL shader attribute.
	class ShaderAttribute
	{

	public:

		ShaderAttribute();
		
		// Set attribute parameters.
		void SetParameters(GLint location, GLint floatCount, GLchar *offset);

		// Enable this shader attribute for rendering.
		void Activate(GLsizei vertexSize) const;

		// Disable this shader attribute from rendering.
		void Deactivate() const;

	private:

		GLint location;
		GLint floatCount;
		GLchar* offset;

	};

}