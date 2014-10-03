#pragma once

#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "image.h"
#include "renderer_shared.h"

namespace Renderer
{

	// Renderer drawing interface.
	class Interface
	{

	public:

		// Initialize renderer.
		virtual bool Initialize() = 0;

		// Shut down renderer.
		virtual void Shutdown() = 0;

		// Clear the scene for a new frame.
		virtual void ClearScene() = 0;

		// Set uniform variable value as float.
		virtual void SetUniform(const Uniform *uniform, float value) = 0;

		// Set uniform variable value as 3-D vector.
		virtual void SetUniform(const Uniform *uniform, const Vector3 *vector) = 0;

		// Set uniform variable value as 4-D vector.
		virtual void SetUniform(const Uniform *uniform, const Vector4 *vector) = 0;

		// Set uniform variable value as 3x3 matrix.
		virtual void SetUniform(const Uniform *uniform, const Matrix3x3 *matrix) = 0;

		// Set uniform variable value as 4x4 matrix.
		virtual void SetUniform(const Uniform *uniform, const Matrix4x4 *matrix) = 0;

	};

}