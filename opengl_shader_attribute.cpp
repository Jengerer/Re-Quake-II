#include "opengl_shader_attribute.h"

namespace OpenGL
{

	// Shader attribute parameter constructor.
	ShaderAttribute::ShaderAttribute(GLint location, GLint floatCount, GLchar *offset)
		: location(location), floatCount(floatCount), offset(offset)
	{
	}

	// Activate this shader attribute for rendering.
	void ShaderAttribute::Activate(GLsizei vertexSize) const
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,
			floatCount,
			GL_FLOAT,
			GL_FALSE,
			vertexSize,
			offset);
	}

	// Deactivate this shader attribute from rendering.
	void ShaderAttribute::Deactivate() const
	{
		glDisableVertexAttribArray(location);
	}

}