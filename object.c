#include "object.h"
#include "memory_manager.h"

/* Null a world object for safe deletion. */
void object_null(object_t *object)
{
	vector3d_clear(&object->origin);
	polygon_null(&object->polygon);

	// Clear the intrusive list nodes.
	object->prev = NULL;
	object->next = NULL;
}

/* Initialize a world object. */
int object_initialize(object_t *object)
{
	(void)object;
	return 1;
}

/* Destroy a world object. */
void object_destroy(object_t* object)
{
	polygon_destroy(&object->polygon);
}
