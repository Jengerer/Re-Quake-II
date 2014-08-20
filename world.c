#include "world.h"
#include "memory_manager.h"

/* Null world for safe deletion. */
void world_null(world_t *world)
{
	world->objects = NULL;
	world->num_objects = 0;
}

/* Initialize the world for some number of objects. */
int world_initialize(world_t *world, int num_objects)
{
	int i;
	object_t *objects;

	// Allocate space for the objects and null them all.
	objects = memory_array_allocate(sizeof(object_t), num_objects);
	if (objects == NULL) {
		return 0;
	}
	for (i = 0; i < num_objects; ++i) {
		object_null(&objects[i]);
	}
	world->objects = objects;
	return 1;
}

/* Destroy the world. */
void world_destroy(world_t *world)
{	
	object_t *objects;

	// Check if anything to destroy.
	objects = world->objects;
	if (objects != NULL) {
		int i, num_objects;

		// Clean up all objects.
		num_objects = world->num_objects;
		for (i = 0; i < num_objects; ++i) {
			object_destroy(&objects[i]);
		}
	}
	memory_free(objects);
}
