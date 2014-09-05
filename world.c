#include "world.h"
#include "memory_manager.h"
#include "object_collision_2d.h"

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

/* Simulate the world physics. */
void world_simulate(world_t *world, float time)
{
	dynamic_object_t *dynamic;

	// Go through each dynamic object...
	for (dynamic = world->dynamic_head; dynamic != NULL; dynamic = dynamic->next) {
		float remaining_time;
		dynamic_object_t *other;
		object_t *object;
		vector3d_t *origin;
		const vector3d_t *velocity;
		trace_result_t trace;

		// Set up no-collision trace.
		object = &dynamic->object;
		origin = &object->origin;
		velocity = &dynamic->velocity;
		trace_result_initialize(&trace, time, velocity);

		// Trace against others.
		remaining_time = time;
		for (other = world->dynamic_head; other != NULL; other = other->next) {
			object_t *other_object;

			// Skip self.
			if (dynamic == other) {
				continue;
			}

			// Trace and update time if collided.
			other_object = &other->object;
			if (object_trace_collision_2d(object, other_object, velocity, remaining_time, &trace)) {
				remaining_time = trace.collision_time;
			}
		}

		// Move the object by the earliest collision.
		vector3d_add(origin, &trace.movement, origin);
	}
}
