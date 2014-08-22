#include "object.h"
#include "memory_manager.h"

/* Null a world object for safe deletion. */
void object_null(object_t *object)
{
	vector3d_clear(&object->origin);
	polygon_null(&object->polygon);
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

/* Null dynamic object for safe deletion. */
void dynamic_object_null(dynamic_object_t *dynamic)
{
	object_null(&dynamic->object);
	vector3d_clear(&dynamic->velocity);

	// Clear linked list nodes.
	dynamic->next = NULL;
	dynamic->prev = NULL;
}

/* Initialize dynamic object. */
int dynamic_object_initialize(dynamic_object_t *dynamic)
{
	if (!object_initialize(&dynamic->object)) {
		return 0;
	}
	return 1;
}

/* Destroy dynamic object. */
void dynamic_object_destroy(dynamic_object_t *dynamic)
{
	object_destroy(&dynamic->object);
	dynamic_object_null(dynamic);
}
