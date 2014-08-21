#include "object_collision_2d.h"

/*
 * Run a collision trace for two objects along the XY plane. 
 * Returns 1 if a collision took place, 0 otherwise.
 */
int object_trace_collision_2d(
	object_t *a,
	object_t *b,
	float time,
	trace_result_t *result)
{
	int collision;
	trace_result_t inner_trace;
	(void)result;

	// Set up result.
	collision = 1;

	// Trace against axes of object A and then B.
	if (!object_trace_against_axes_2d(a, b, a, time, &inner_trace)) {
		collision = 0;
	}
	else if (!object_trace_against_axes_2d(a, b, b, time, &inner_trace)) {
		collision = 0;
	}

	return collision;
}

/*
 * Run a collision trace for two objects using one of their separating axes.
 * Returns 1 if a collision took place, 0 otherwise.
 */
int object_trace_against_axes_2d(
	object_t *a,
	object_t *b,
	object_t *object_axes,
	float time,
	trace_result_t *result)
{
	int i, num_axes;
	int did_collide;
	const polygon_t *axes_polygon;
	(void)time;
	(void)result;
	
	// Go through each axis.
	did_collide = 1;
	axes_polygon = &object_axes->polygon;
	num_axes = polygon_get_num_vertices(axes_polygon);
	for (i = 0; i < num_axes; ++i) {
		float a_min, a_max;
		float b_min, b_max;
		vector3d_t axis;

		// Get the axis and project both objects.
		object_get_separating_axis(object_axes, i, &axis);

		// Project the two objects.
		object_project_to_axis(a, &axis, &a_min, &a_max);
		object_project_to_axis(b, &axis, &b_min, &b_max);

		// Check intersection and fill out result/time.
		if ((a_max > b_min) && (a_max <= b_max)) {
			// Collision happened.
		}
		else if ((a_min >= b_min) && (a_min < b_max)) {
			// Collision happened.
		}
		else {
			// Found separating axis, no collision.
			did_collide = 0;
		}
	}

	return did_collide;
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
