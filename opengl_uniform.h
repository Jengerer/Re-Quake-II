#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Object representing a uniform variable in an OpenGL shader.
	class Uniform : public Renderer::Uniform
	{

	public:

		Uniform(GLint location);

		// Numeric value setters.

		void SetValue(float value) const;

		// Vector setters.

		void SetValue(const Vector3 *vector) const;
		void SetValue(const Vector4 *vector) const;

		// Matrix setters.

		void SetValue(const Matrix3x3 *matrix) const;
		void SetValue(const Matrix4x4 *matrix) const;

	private:

		GLint location;

	};

}