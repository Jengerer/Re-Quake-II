#include "map.h"
#include "memory_manager.h"
#include <stdio.h> // TODO: Maybe remove.

/*
 * Base initialization to have no objects.
 */
void map_null(map_t *map)
{
	map->objects = NULL;
	map->num_objects = 0;
}

/*
 * Allocate space for a certain number of objects.
 */
int map_initialize(map_t *map, int num_objects)
{
	int i;
	object_t *objects;

	// Allocate space for polygons and null them.
	objects = (object_t*)memory_array_allocate(sizeof(object_t), num_objects);
	if (objects == NULL) {
		return 0;
	}
	for (i = 0; i < num_objects; ++i) {
		object_null(&objects[i]);
	}

	// Fill out struct and return.
	map->objects = objects;
	map->num_objects = num_objects;
	return 1;
}

/*
 * Deallocate all polygons.
 */
void map_destroy(map_t *map)
{
	int i;
	for (i = 0; i < map->num_objects; ++i) {
		object_destroy(&map->objects[i]);
	}
	memory_free(map->objects);
	map_null(map);
}
