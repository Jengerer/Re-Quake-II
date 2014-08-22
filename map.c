#include "map.h"
#include "memory_manager.h"

/*
 * Base initialization to have no objects.
 */
void map_null(map_t *map)
{
	map->head = NULL;
}

/*
 * Allocate space for a certain number of objects.
 */
int map_initialize(map_t *map)
{
	(void)map;
	return 1;
}

/*
 * Deallocate all map objects.
 */
void map_destroy(map_t *map)
{
	map_object_t *node;
	
	// Delete all objects.
	node = map->head;
	while (node != NULL) {
		map_object_t *next = node->next;

		// Deallocate object and node.
		object_destroy(&node->object);
		memory_free(node);
		node = next;
	}
}
