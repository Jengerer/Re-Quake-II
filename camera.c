#include "camera.h"

/* Update camera position. */
void camera_set_origin(camera_t *camera, const vector3d_t *origin)
{
	entity_t *camera_entity;
	vector3d_t *camera_origin;
	
	camera_entity = &camera->entity;
	camera_origin = &camera_entity->origin;
	vector3d_copy(origin, camera_origin);
}

/* Update camera angles. */
void camera_set_angles(camera_t *camera, const vector3d_t *angles)
{
	entity_t *camera_entity;
	vector3d_t *camera_angles;
	
	camera_entity = &camera->entity;
	camera_angles = &camera_entity->angles;
	vector3d_copy(angles, camera_angles);
}

/* Get the 4x4 matrix representing camera view transform. */
void camera_world_to_view_transform(camera_t *camera, matrix4x4_t *out)
{
	entity_t *camera_entity;
	vector3d_t *camera_origin, *camera_angles;
	vector3d_t transform_offset, transform_angles;
	matrix4x4_t rotation, translation;

	// Get camera origin and angles.
	camera_entity = &camera->entity;
	camera_origin = &camera_entity->origin;
	camera_angles = &camera_entity->angles;

	// Get translation matrix; negate camera position.
	vector3d_negate(camera_origin, &transform_offset);
	matrix4x4_translation(&transform_offset, &translation);

	// Calculate the full rotation matrix; negate camera angles.
	vector3d_negate(camera_angles, &transform_angles);
	matrix4x4_rotation_euler(&transform_angles, &rotation);

	// Multiply for result.
	matrix4x4_multiply(&rotation, &translation, out);
}
