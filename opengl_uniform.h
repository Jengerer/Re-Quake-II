#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Object representing a uniform variable in an OpenGL shader.
	class Uniform
	{

	public:

		Uniform(GLint location);

		// Numeric value setters.

		void SetValue(float value);

		// Vector setters.

		void SetValue(const Vector3 *vector);
		void SetValue(const Vector4 *vector);

		// Matrix setters.

		void SetValue(const Matrix3x3 *matrix);
		void SetValue(const Matrix4x4 *matrix);

	private:

		GLint location;

	};

}