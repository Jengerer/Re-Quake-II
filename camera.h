#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "entity.h"
#include "matrix4x4.h"

// Structure for representing a point of view.
typedef struct camera
{
	vector3d_t origin;
	vector3d_t angles;
} camera_t;

// Camera position and angle functions.
void camera_set_origin(camera_t *camera, const vector3d_t *origin);
void camera_set_angles(camera_t *camera, const vector3d_t *angles);

// Get the matrix transform for camera view.
void camera_world_to_view_transform(camera_t *camera, matrix4x4_t *out);

#endif // _CAMERA_H_