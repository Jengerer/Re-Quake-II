#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Object for representing an OpenGL shader attribute.
	class ShaderAttribute
	{

	public:

		ShaderAttribute(GLint location, GLchar* offset, Renderer::DataType type);

		// Enable this shader attribute for rendering.
		void Activate() const;

		// Disable this shader attribute from rendering.
		void Deactivate() const;

	public:

		// Get size of base data type for a type (e.g. size of float for 3-D vector).
		static GLsizei GetDataTypeSize(Renderer::DataType type);

	private:

		// Convert data type from renderer type to its base OpenGL data type (vector to GL_FLOAT).
		static GLenum GetElementType(Renderer::DataType type);

		// Get number of base data elements in a type (e.g. 3 floats in a 3-D vector).
		static GLsizei GetElementCount(Renderer::DataType type);

	private:

		GLint location;
		GLenum elementType;
		GLint elementCount;
		GLchar* offset;

	};

}