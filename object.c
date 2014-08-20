#include "object.h"

/* Null a world object for safe deletion. */
void object_null(object_t *object)
{
	vector3d_clear(&object->origin);
	polygon_null(&object->polygon);
}

/* Initialize a world object. */
int object_initialize(object_t *object, int num_vertices)
{
	// Initialize the polygon.
	if (!polygon_initialize(&object->polygon, num_vertices)) {
		return 0;
	}
	return 1;
}

/* Destroy a world object. */
void object_destroy(object_t* object)
{
	polygon_destroy(&object->polygon);
}
