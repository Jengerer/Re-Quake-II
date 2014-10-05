#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Object representing a uniform variable in an OpenGL shader.
	class Uniform : public Renderer::Uniform
	{

	public:

		Uniform(GLint location);

		// Set value as floating point.
		void SetValue(float value) const;

		// Set value as 3-D vector.
		void SetValue(const Vector3 *vector) const;

		// Set value as 4-D vector.
		void SetValue(const Vector4 *vector) const;

		// Set value as 3x3 matrix.
		void SetValue(const Matrix3x3 *matrix) const;

		// Set value as 4x4 matrix.
		void SetValue(const Matrix4x4 *matrix) const;

	private:

		GLint location;

	};

}