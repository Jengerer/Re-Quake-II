#include "camera.h"
#include <math_common.h>

// Camera angle limits.
const float CameraMaximumPitch = 89.f;
const float CameraMinimumPitch = -CameraMaximumPitch;

Camera::Camera()
{
	angles.Clear();
}

// Get forward direction for camera.
void Camera::GetDirections(Vector3 *forward, Vector3 *right, Vector3 *up)
{
	angles.AnglesToVectors(forward, right, up);
}

// Set frustum parameters and update projection transform.
void Camera::SetFrustum(float nearZ, float farZ, float aspectRatio, float fieldOfView)
{
	// TODO: May not need some of these parameters to get near plane world size; figure out which.
	this->nearZ = nearZ;
	this->farZ = farZ;
	this->aspectRatio = aspectRatio;
	this->fieldOfView = fieldOfView;
	projection.PerspectiveProjection(aspectRatio, fieldOfView, nearZ, farZ);
}

// Turn the angle by a set of angles.
void Camera::Turn(const Vector3 &turnAngles)
{
	angles.x = MathCommon::ClampAngle(angles.x + turnAngles.x);
	if (angles.x > CameraMaximumPitch) {
		angles.x = CameraMaximumPitch;
	}
	else if (angles.x < CameraMinimumPitch) {
		angles.x = CameraMinimumPitch;
	}
	angles.y = MathCommon::ClampAngle(angles.y + turnAngles.y);
}

// Generate world to camera view transform.
void Camera::GenerateProjectionView(Matrix4x4 *out)
{
	Vector3 translateOffset;
	Matrix4x4 rotation, translation, view;

	// Get the inverse (transpose) rotation matrix.
	rotation.RotationEuler(&angles);
	rotation.Transpose(&rotation);
	
	// Get the inverse translation matrix.
	translateOffset.Negation(position);
	translation.Translation(&translateOffset);

	// Multiply the two to get view.
	view.Product(&rotation, &translation);

	// Combine view and projection.
	out->Product(&projection, &view);
}
