#pragma once

#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix3x3.h>
#include <matrix4x4.h>

namespace Renderer
{

	// Class for managing a material property.
	class Variable
	{

	public:

		// Destroy the reference to the variable.
		virtual void Destroy() = 0;

		// Set value as floating point.
		virtual void Set(float value) = 0;

		// Set value as 2-D vector.
		virtual void Set(const Vector2 *value) = 0;

		// Set value as 3-D vector.
		virtual void Set(const Vector3 *value) = 0;

		// Set value as 4-D vector.
		virtual void Set(const Vector4 *value) = 0;

		// Set value as 3x3 matrix.
		virtual void Set(const Matrix3x3 *value) = 0;

		// Set value as 4x4 matrix.
		virtual void Set(const Matrix4x4 *value) = 0;

	};

}