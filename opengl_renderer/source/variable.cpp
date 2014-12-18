#include "variable.h"
#include <memory_manager.h>

namespace OpenGL
{

	Variable::Variable(GLint location) : location(location)
	{
	}

	// Destroy variable reference.
	void Variable::Destroy()
	{
		delete this;
	}

	// Set value as integer.
	void Variable::SetInteger(int value)
	{
		glUniform1i(location, value);
	}

	// Set value as float.
	void Variable::SetFloat(float value)
	{
		glUniform1f(location, value);
	}

	// Set value as 2-D vector.
	void Variable::SetVector2(const Vector2 *value)
	{
		glUniform2fv(location, 1, static_cast<const GLfloat*>(&value->x));
	}

	// Set value as 3-D vector.
	void Variable::SetVector3(const Vector3 *value)
	{
		glUniform3fv(location, 1, static_cast<const GLfloat*>(&value->x));
	}

	// Set value as 4-D vector.
	void Variable::SetVector4(const Vector4 *value)
	{
		glUniform4fv(location, 1, static_cast<const GLfloat*>(&value->x));
	}

	// Set value as 3x3 matrix.
	void Variable::SetMatrix3x3(const Matrix3x3 *value)
	{
		glUniformMatrix3fv(location, 1, GL_TRUE, static_cast<const GLfloat*>(&value->matrixArray[0][0]));
	}

	// Set value as 4x4 matrix.
	void Variable::SetMatrix4x4(const Matrix4x4 *value)
	{
		glUniformMatrix4fv(location, 1, GL_TRUE, static_cast<const GLfloat*>(&value->matrixArray[0][0]));
	}

	// Private destructor; must be killed through Destroy().
	Variable::~Variable()
	{
	}

}
