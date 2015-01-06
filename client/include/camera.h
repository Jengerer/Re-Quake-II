#pragma once

#include <matrix4x4.h>

// Structure for representing a point of view.
class Camera
{

public:

	Camera();

	// Camera parameter functions.
	inline const Vector3 *GetPosition() const { return &position; }
	inline const Vector3 *GetAngles() const { return &angles; }
	inline void SetPosition(const Vector3 &position) { this->position = position; }
	inline void SetAngles(const Vector3 &angles) { this->angles = angles; }

	// Update frustum and cache transform.
	void SetFrustum(float nearZ, float farZ, float aspectRatio, float fieldOfView);

	// Get camera direction.
	// The output parameter may be null.
	void GetDirections(Vector3 *forward, Vector3 *right, Vector3 *up);

	// Camera parameter alteration.
	void Turn(const Vector3& turnAngles);

	// Generate camera transform.
	void GenerateProjectionView(Matrix4x4 *out);

private:

	// Camera world orientation and position.
	Vector3 position;
	Vector3 angles;

	// Camera frustum parameters.
	Matrix4x4 projection;
	float nearZ;
	float farZ;
	float aspectRatio;
	float fieldOfView;

};