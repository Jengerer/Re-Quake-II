#include "camera.h"

/* Update camera position. */
void camera_set_origin(camera_t *camera, const vector3d_t *origin)
{
	vector3d_t *camera_origin;
	camera_origin = &camera->origin;
	vector3d_copy(origin, camera_origin);
}

/* Update camera angles. */
void camera_set_angles(camera_t *camera, const vector3d_t *angles)
{
	vector3d_t *camera_angles;
	camera_angles = &camera->angles;
	vector3d_copy(angles, camera_angles);
}

/* Get the 4x4 matrix representing camera view transform. */
void camera_world_to_view_transform(camera_t *camera, matrix4x4_t *out)
{
	vector3d_t *camera_origin, *camera_angles;
	vector3d_t transform_offset;
	matrix4x4_t rotation, translation;

	// Get camera origin and angles.
	camera_origin = &camera->origin;
	camera_angles = &camera->angles;

	// Get the inverse (transpose) rotation matrix.
	matrix4x4_rotation_euler(camera_angles, &rotation);
	matrix4x4_transpose(&rotation, &rotation);

	// Get the inverse translation matrix.
	vector3d_negate(camera_origin, &transform_offset);
	matrix4x4_translation(&transform_offset, &translation);

	// Multiply the two.
	matrix4x4_multiply(&rotation, &translation, out);
}
