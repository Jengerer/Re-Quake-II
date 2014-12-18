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

		// Set value as integer.
		virtual void SetInteger(int value) = 0;

		// Set value as floating point.
		virtual void SetFloat(float value) = 0;

		// Set value as 2-D vector.
		virtual void SetVector2(const Vector2 *value) = 0;

		// Set value as 3-D vector.
		virtual void SetVector3(const Vector3 *value) = 0;

		// Set value as 4-D vector.
		virtual void SetVector4(const Vector4 *value) = 0;

		// Set value as 3x3 matrix.
		virtual void SetMatrix3x3(const Matrix3x3 *value) = 0;

		// Set value as 4x4 matrix.
		virtual void SetMatrix4x4(const Matrix4x4 *value) = 0;

	};

}