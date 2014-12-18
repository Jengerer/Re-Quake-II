#pragma once

#include "common.h"
#include <allocatable.h>
#include <renderer/variable_interface.h>

namespace OpenGL
{

	// Object representing a uniform variable in an OpenGL program.
	class Variable : public Renderer::Variable, public Allocatable
	{

	public:

		Variable(GLint location);

		// Destroy the reference to this variable.
		virtual void Destroy();

		// Set value as integer.
		virtual void SetInteger(int value);

		// Set value as floating point.
		virtual void SetFloat(float value);

		// Set value as 2-D vector.
		virtual void SetVector2(const Vector2 *value);

		// Set value as 3-D vector.
		virtual void SetVector3(const Vector3 *value);

		// Set value as 4-D vector.
		virtual void SetVector4(const Vector4 *value);

		// Set value as 3x3 matrix.
		virtual void SetMatrix3x3(const Matrix3x3 *value);

		// Set value as 4x4 matrix.
		virtual void SetMatrix4x4(const Matrix4x4 *value);

	private:

		// Private destructor; must be killed through Destroy().
		~Variable();

	private:

		GLint location;

	};

}