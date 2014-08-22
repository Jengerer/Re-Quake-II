#ifndef _OBJECT_COLLISION_2D_H_
#define _OBJECT_COLLISION_2D_H_

#include "object.h"

// Trace result structure.
typedef struct trace_result
{
	float collision_time;
	vector3d_t movement;
	vector3d_t normal;
} trace_result_t;

// Object collision tracing along the XY plane.
int object_trace_collision_2d(
	object_t *a,
	object_t *b,
	float time,
	trace_result_t *result);

// Trace movement of an object against the separating axes of another.
int object_trace_against_axes_2d(
	object_t *a,
	object_t *b,
	object_t *object_axes,	
	float time,
	trace_result_t *result);

// Helper function for getting a separating axis from an object.
void object_get_separating_axis(
	object_t *object_axes,
	int index,
	vector3d_t *out);

// Helper function for projecting an object's vertices onto an axis.
void object_project_to_axis(
	object_t *object,
	const vector3d_t *axis,
	float *min_out,
	float *max_out);

#endif // _OBJECT_COLLISION_2D_H_