#include "object_collision_2d.h"

/* Initialize a trace result to full time movement. */
void trace_result_initialize(trace_result_t *trace, float time, const vector3d_t *velocity)
{
	trace->collision_time = time;
	vector3d_scale(velocity, time, &trace->movement);
}

/*
 * Run a collision trace for two objects along the XY plane. 
 * Returns 1 if a collision took place, 0 otherwise.
 */
int object_trace_collision_2d(
	object_t *a,
	object_t *b,
	const vector3d_t *velocity,
	float time,
	trace_result_t *result)
{
	trace_result_t inner_trace;
	int collided;

	// Set up result and time sentinel.
	inner_trace.collision_time = time;
	vector3d_scale(velocity, time, &inner_trace.movement);

	// Trace against axes of object A and then B.
	collided = 1;
	if (!object_trace_against_axes_2d(a, b, a, velocity, time, &inner_trace)) {
		collided = 0;
	}
	else if (!object_trace_against_axes_2d(a, b, b, velocity, time, &inner_trace)) {
		collided = 0;
	}
	
	// Copy trace result.
	result->collision_time = inner_trace.collision_time;
	vector3d_copy(&inner_trace.movement, &result->movement);
	return collided;
}

/*
 * Run a collision trace for two objects using one of their separating axes.
 * Returns 1 if a collision took place, 0 otherwise.
 */
int object_trace_against_axes_2d(
	object_t *a,
	object_t *b,
	object_t *object_axes,
	const vector3d_t *velocity,
	float time,
	trace_result_t *result)
{
	int i, num_axes;
	const polygon_t *axes_polygon;
	trace_result_t inner_trace;
	
	// Set up trace.
	inner_trace.collision_time = -1.0f;

	// Go through each axis.
	axes_polygon = &object_axes->polygon;
	num_axes = polygon_get_num_vertices(axes_polygon);
	for (i = 0; i < num_axes; ++i) {
		float a_min, a_max;
		float b_min, b_max;
		float velocity_dot;
		vector3d_t axis;

		// Get the axis and project both objects.
		object_get_separating_axis(object_axes, i, &axis);

		// Project the two objects.
		object_project_to_axis(a, &axis, &a_min, &a_max);
		object_project_to_axis(b, &axis, &b_min, &b_max);

		// Check intersection and fill out result/time.
		velocity_dot = vector3d_dot_product(velocity, &axis);
		if (a_max <= b_min) {
			// No collision yet, but check for future.
			if (velocity_dot > 0.0f) {
				float hit_time = (b_min - a_max) / (velocity_dot);
				if (hit_time < time) {
					// We can hit, but is this the furthest collision?
					if (hit_time > inner_trace.collision_time) {
						inner_trace.collision_time = hit_time;
						vector3d_scale(velocity, hit_time, &inner_trace.movement);
					}
					continue;
				}
			}
			return 0;
		}
		else if (a_min >= b_max) {
			// No collision yet, but check for future.
			if (velocity_dot < 0.0f) {
				float hit_time = time * (b_max - a_min) / (velocity_dot);
				if (hit_time < time) {
					// We can hit, but is this the furthest collision?
					if (hit_time > result->collision_time) {
						inner_trace.collision_time = hit_time;
						vector3d_scale(velocity, hit_time, &inner_trace.movement);
					}
					continue;
				}
			}
			return 0;
		}
		else if (inner_trace.collision_time < 0.0f) {
			// Already colliding.
			inner_trace.collision_time = 0.0f;
			vector3d_clear(&inner_trace.movement);
		}
	}

	// Collision on all axes, return result.
	result->collision_time = inner_trace.collision_time;
	vector3d_copy(&inner_trace.movement, &result->movement);
	return 1;
}

/*
 * Get a normalized vector for the separating axis.
 * Assumes the object has at least 3 vertices and valid index.
 */
void object_get_separating_axis(
	object_t *object_axes,
	int index,
	vector3d_t *out)
{
	int start, end, num_vertices;
	polygon_t *polygon;
	mesh_vertex_t *start_vertex, *end_vertex;

	// Calculate indices for vertices.
	polygon = &object_axes->polygon;
	num_vertices = polygon_get_num_vertices(polygon);
	start = index;
	end = (index + 1) % num_vertices;

	// Get vertices and get difference.
	start_vertex = polygon_get_vertex(polygon, start);
	end_vertex = polygon_get_vertex(polygon, end);
	vector3d_subtract(&end_vertex->position, &start_vertex->position, out);
	vector3d_normalize(out, out);
}

/*
 * Project an object's vertices onto axis and return min/max.
 * The object must have at least one vertex. 
 */
void object_project_to_axis(
	object_t *object,
	const vector3d_t *axis,
	float *min_out,
	float *max_out)
{
	int i, num_vertices;
	float min, max;
	float offset;
	polygon_t *polygon;
	mesh_vertex_t *mesh_vertex;

	// Project first vertex for min/max base.
	polygon = &object->polygon;
	mesh_vertex = polygon_get_vertex(polygon, 0);
	min = max = vector3d_dot_product(&mesh_vertex->position, axis);

	// Project vertices, skip first.
	num_vertices = polygon_get_num_vertices(polygon);
	for (i = 1; i < num_vertices; ++i) {
		// Project onto axis and update min/max.
		mesh_vertex = polygon_get_vertex(polygon, i);
		float dot = vector3d_dot_product(&mesh_vertex->position, axis);
		if (dot < min) {
			min = dot;
		}
		else if (dot > max) {
			max = dot;
		}
	}

	// Project offset onto axis and add to min/max.
	offset = vector3d_dot_product(&object->origin, axis);

	// Update min/max output.
	*min_out = min + offset;
	*max_out = max + offset;
}
