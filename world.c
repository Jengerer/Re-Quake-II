#include "world.h"
#include "memory_manager.h"

/* Null world for safe deletion. */
void world_null(world_t *world)
{
	world->head = NULL;
}

/* Initialize the world for some number of objects. */
int world_initialize(world_t *world)
{
	(void)world;
	return 1;
}

/* Destroy the world. */
void world_destroy(world_t *world)
{	
	object_t *object;

	// Destroy the objects.
	object = world->head;
	while (object != NULL) {
		object_t *next = object->next;
		object_destroy(object);
		object = next;
	}
	world_null(world);
}

/*
 * Create an object and add it to the world.
 * Returns the object on success, NULL otherwise.
 */
object_t* world_create_object(world_t *world)
{
	object_t *object;

	// Allocate an object.
	object = memory_allocate(sizeof(object_t));
	if (object == NULL) {
		return NULL;
	}
	object_null(object);

	// Initialize object.
	if (!object_initialize(object)) {
		object_destroy(object);
		return NULL;
	}

	// Add it to the list and return.
	object->next = world->head;
	world->head = object;
	return object;
}

/* Remove and destroy the object from the world. */
void world_destroy_object(world_t *world, object_t *object)
{
	// Check if we need to update the head.
	if (world->head == object) {
		world->head = object->next;
	}

	// Remove object from the chain.
	if (object->next != NULL) {
		object->next->prev = object->prev;
	}
	if (object->prev != NULL) {
		object->prev->next = object->next;
	}
}
