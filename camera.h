#pragma once

#include "matrix4x4.h"

// Structure for representing a point of view.
class Camera
{

public:

	Camera();

	// Set position and angles.
	void SetPosition(const Vector3 *position);
	void SetAngles(const Vector3 *angles);

	// Generate camera transform.
	void GenerateViewTransform(Matrix4x4 *out);

private:

	Vector3 position;
	Vector3 angles;

};