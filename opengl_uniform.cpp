#include "opengl_uniform.h"

namespace OpenGL
{

	Uniform::Uniform(GLint location) : location(location)
	{
	}

	// Set value as float.
	void Uniform::SetValue(float value) const
	{
		glUniform1f(location, value);
	}

	// Set value as 3-D vector.
	void Uniform::SetValue(const Vector3 *vector) const
	{
		glUniform3fv(location, 1, static_cast<const GLfloat*>(&vector->x));
	}

	// Set value as 4-D vector.
	void Uniform::SetValue(const Vector4 *vector) const
	{
		glUniform4fv(location, 1, static_cast<const GLfloat*>(&vector->x));
	}

	// Set value as 3x3 matrix.
	void Uniform::SetValue(const Matrix3x3 *matrix) const
	{
		glUniformMatrix3fv(location, 1, GL_TRUE, static_cast<const GLfloat*>(&matrix->matrixArray[0][0]));
	}

	// Set value as 4x4 matrix.
	void Uniform::SetValue(const Matrix4x4 *matrix) const
	{
		glUniformMatrix4fv(location, 1, GL_TRUE, static_cast<const GLfloat*>(&matrix->matrixArray[0][0]));
	}

}
