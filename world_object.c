#include "world_object.h"

/* Null a world object for safe deletion. */
void world_object_null(world_object_t *object)
{
	vector3d_clear(&object->origin);
	polygon_null(&object->polygon);
}

/* Initialize a world object. */
int world_object_initialize(world_object_t *object, int num_vertices)
{
	// Initialize the polygon.
	if (!polygon_initialize(&object->polygon, num_vertices)) {
		return 0;
	}
	return 1;
}

/* Destroy a world object. */
void world_object_destroy(world_object_t* object)
{
	polygon_destroy(&object->polygon);
}
