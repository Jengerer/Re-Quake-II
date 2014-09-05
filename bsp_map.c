#include "bsp_map.h"
#include "memory_manager.h"

/* Null a BSP map for safe destruction. */
void bsp_map_null(bsp_map_t *map)
{
	bsp_map_null_planes(map);
}

/* Destroy a BSP map. */
void bsp_map_destroy(bsp_map_t *map)
{
	bsp_map_destroy_planes(map);
}

/* Null the planes array for BSP map. */
void bsp_map_null_planes(bsp_map_t *map)
{
	map->planes = NULL;
	map->num_planes = 0;
}

/* Initialize the array for BSP splitting planes. */
int bsp_map_initialize_planes(bsp_map_t *map, int num_planes)
{
	bsp_plane_t *planes;

	// Allocate space for the array.
	planes = (bsp_plane_t*)memory_array_allocate(sizeof(bsp_plane_t), num_planes);
	if (planes == NULL) {
		return 0;
	}
	map->planes = planes;
	map->num_planes = num_planes;
	return 1;
}

/* Destroy the array for BSP splitting planes. */
void bsp_map_destroy_planes(bsp_map_t *map)
{
	bsp_plane_t *planes;

	// Just deallocate the array if it exists.
	planes = map->planes;
	if (planes != NULL) {
		memory_free(planes);
	}
	bsp_map_null_planes(map);
}
