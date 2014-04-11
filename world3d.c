#include "world3d.h"
#include <stdlib.h>

/*
 * Allocate space for objects in the world.
 * Returns 0 on failure, 1 otherwise.
 */
int initialize_world(world2d_t *world, int object_max)
{
	// Allocate array of objects.
	object2d_t *objects = (object2d_t*)malloc(object_max * sizeof(object2d_t));
	if (objects == NULL) {
		return 0;
	}
	world->object_count = 0;
	world->object_max = object_max;
	return 1;
}

/*
 * Deallocate the objects in the world.
 */
void destroy_world(world2d_t *world)
{
	// Clear all objects in the world.
	int i;
	object2d_t *object;
	for (i = 0; i < world->object_count; ++i) {
		object = &world->objects[i];
		destroy_object(object);
	}
	free(world->objects);
}

/*
 * Allocate an object from the world's pool.
 * Returns a pointer into the pool on success, NULL otherwise.
 */
object2d_t *allocate_object(world2d_t *world)
{
	// Check if we have space.
	object2d_t *object;
	int index = world->object_count;
	if (index == world->object_max) {
		return NULL;
	}

	// Null object at index and return.
	object = &world->objects[index];
	null_object(object);
	return object;
}

/*
 * Check intersection of an object against another across a given normal.
 * Returns 1 if an intersection took place, 0 otherwise.
 * Float at time_out is filled out with the time of intersection along displacement.
 */
int intersect_normal_test(const object2d_t *a, const object2d_t *b, const vector2d_t *normal,
	const vector2d_t *displacement, float *time_out)
{
	int j;
	float projection;
	float min_a;
	float max_a;
	float min_b;
	float max_b;
	const vector2d_t *vertex;

	// Project A's vertices in its object space.
	for (j = 0; j < polygon_a->vertex_count; ++j) {
		vertex = &polygon_a->vertices[j];
		projection = vector_dot_product(vertex, normal);
		if (j == 0) {
			min_a = projection;
			max_a = projection;
		}
		else if (projection < min_a) {
			min_a = projection;
		}
		else if (projection > max_a) {
			max_a = projection;
		}
	}

	// Convert velocity to object A's space and add to max projection.
	vector_rotate(displacement, a->rotation, &displacement_a);
	max_a += vector_dot_product(&displacement_a, normal);

	// Project the other object's transformed points onto the normal.
	for (j = 0; j < polygon_b->vertex_count; ++j) {
		vector2d_t transformed;
		vertex = &polygon_b->vertices[j];

		// Transform coordinates into A's object space.
		vector_rotate(vertex, b->rotation, &transformed);
		vector_add(&transformed, &offset, &transformed);
		vector_rotate(vertex, a->rotation, &transformed);

		// Project onto the normal.
		projection = vector_dot_product(vertex, &normal);
		if (j == 0) {
			min_b = projection;
			max_b = projection;
		}
		else if (projection < min_b) {
			min_b = projection;
		}
		else if (projection > max_b) {
			max_b = projection;
		}
	}

	// Check for projection intersection.

}


/*
 * Check intersection of an object against another.
 * Fills out trace structure if earlier intersection detected.
 */
void intersect_test(const object2d_t *a, const object2d_t *b,
	const vector2d_t *displacement, trace2d_t *out)
{
	int i;
	int edges;
	const polygon2d_t *polygon_a;
	const polygon2d_t *polygon_b;
	vector2d_t normal;
	vector2d_t offset;
	vector2d_t displacement_a;
	const vector2d_t *vertex;
	polygon_a = &a->polygon;
	polygon_b = &b->polygon;

	// Calculate offset from A to B.
	vector_subtract(&b->position, &a->position, &offset);

	// Test first object's normals.
	edges = get_polygon_edge_count(polygon_a);
	for (i = 0; i < edges; ++i) {
		get_polygon_edge_normal(polygon_a, i, &normal);

		// Project all of the object's points onto the normal.
	}
}

/*
 * Check collision of object between two points in the world.
 */
void trace_object(const world2d_t* world, const object2d_t *object,
	const vector2d_t *displacement, trace2d_t *out)
{

	// Variables for per-object checks.
	int i;
	float fraction;
	vector2d_t normal;
	polygon2d_t *object_polygon;
	polygon2d_t *other_polygon;

	// Start fresh; no collision yet.
	out->fraction = 1.0f;
	out->hit_object = NULL;
	
	// Go through each world object (except for tested object).
	for (i = 0; i < world->object_count; ++i) {
		int j;
		object2d_t *other = &world->objects[i];

		// Skip tested object.
		if (object == other) {
			continue;
		}

		// First check against tested object's normals.
		trace_object_normals(object, other, displacement, out);
	}
	
}

