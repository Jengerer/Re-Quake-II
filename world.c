#include "world.h"
#include "memory_manager.h"

/* Null world for safe deletion. */
void world_null(world_t *world)
{
	world->dynamic_head = NULL;
	map_null(&world->map);
}

/* Initialize the world for some number of objects. */
int world_initialize(world_t *world)
{
	// Initialize the map.
	if (!map_initialize(&world->map)) {
		return 0;
	}
	return 1;
}

/* Destroy the world. */
void world_destroy(world_t *world)
{	
	dynamic_object_t *object;

	// Destroy the objects.
	object = world->dynamic_head;
	while (object != NULL) {
		dynamic_object_t *next = object->next;
		dynamic_object_destroy(object);
		memory_free(object);
		object = next;
	}
	world_null(world);
}

/*
 * Create an object and add it to the world.
 * Returns the object on success, NULL otherwise.
 */
dynamic_object_t* world_create_dynamic_object(world_t *world)
{
	dynamic_object_t *dynamic;

	// Allocate an object.
	dynamic = memory_allocate(sizeof(dynamic_object_t));
	if (dynamic == NULL) {
		return NULL;
	}
	dynamic_object_null(dynamic);

	// Initialize object.
	if (!dynamic_object_initialize(dynamic)) {
		dynamic_object_destroy(dynamic);
		memory_free(dynamic);
		return NULL;
	}

	// Add it to the list and return.
	dynamic->next = world->dynamic_head;
	world->dynamic_head = dynamic;
	return dynamic;
}

/* Remove and destroy the object from the world. */
void world_destroy_dynamic_object(world_t *world, dynamic_object_t *dynamic)
{
	// Check if we need to update the head.
	if (world->dynamic_head == dynamic) {
		world->dynamic_head = dynamic->next;
	}

	// Remove object from the chain.
	if (dynamic->next != NULL) {
		dynamic->next->prev = dynamic->prev;
	}
	if (dynamic->prev != NULL) {
		dynamic->prev->next = dynamic->next;
	}
}
