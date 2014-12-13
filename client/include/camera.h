#pragma once

#include <matrix4x4.h>

// Structure for representing a point of view.
class Camera
{

public:

	Camera();

	// Set camera world position.
	void SetPosition(const Vector3 &position);

	// Set camera angles.
	void SetAngles(const Vector3 &angles);

	// Generate camera transform.
	void GenerateViewTransform(Matrix4x4 *out);

private:

	Vector3 position;
	Vector3 angles;

};