#include "camera.h"

Camera::Camera()
{
}

// Update camera world position.
void Camera::SetPosition(const Vector3 &position)
{
	this->position = position;
}


// Update camera angles.
void Camera::SetAngles(const Vector3 &angles)
{
	this->angles = angles;
}

// Generate world to camera view transform.
void Camera::GenerateViewTransform(Matrix4x4 *out)
{
	Vector3 translateOffset;
	Matrix4x4 rotation, translation;

	// Get the inverse (transpose) rotation matrix.
	rotation.RotationEuler(&angles);
	rotation.Transpose(&rotation);
	
	// Get the inverse translation matrix.
	translateOffset.Negation(&position);
	translation.Translation(&translateOffset);

	// Multiply the two.
	out->Product(&rotation, &translation);
}
