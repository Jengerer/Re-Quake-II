#pragma once

#include "common.h"

namespace OpenGL
{

	// Object for representing an OpenGL program attribute.
	class Attribute
	{

	public:

		Attribute();

		// Set attribute parameters.
		// Returns the size of this attribute.
		int SetParameters(GLint location, GLchar *offset, Renderer::DataType type);

		// Enable this shader attribute for rendering.
		void Activate(int stride) const;

		// Disable this shader attribute from rendering.
		void Deactivate() const;

	public:

		// Get size of base element type for a data type (size of float for vector).
		static GLsizei GetElementSize(Renderer::DataType type);

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
