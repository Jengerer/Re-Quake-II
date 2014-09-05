#include "bsp_map_parser.h"
#include "error_stack.h"
#include "file.h"

// Private parsing functions.
static int bsp_map_parse(const file_t *bsp_file, bsp_map_t *out);
static int bsp_map_parse_planes(const bsp_header_t *header, bsp_map_t*);

/*
 * Load and parse a BSP from a file.
 * Returns 1 on success, 0 otherwise.
 */
int bsp_map_load(const char *filename, bsp_map_t *out)
{
	int result;
	file_t bsp_file;

	// Open the file.
	file_null(&bsp_file);
	if (file_load(filename, &bsp_file)) {
		result = bsp_map_parse(&bsp_file, out);
	}
	else {
		result = 0;
	}

	// Free the file.
	file_destroy(&bsp_file);
	return result;
}

/*
 * Private parsing function once the file is loaded.
 * Returns 1 on success, 0 otherwise.
 */
static int bsp_map_parse(const file_t *bsp_file, bsp_map_t *map)
{
	const bsp_header_t *header;

	// Get header and verify magic number/version.
	header = (const bsp_header_t*)bsp_file->buffer;
	if (header->magic != BSP_HEADER_MAGIC) {
		error_stack_log("Error parsing BSP header: magic number mismatch.");
		return 0;
	}
	if (header->version != BSP_VERSION) {
		error_stack_log("Error parsing BSP header: unexpected version.");
		return 0;
	}

	// Parse different components.
	if (!bsp_map_parse_planes(header, map)) {
		return 0;
	}
	return 1;
}

/*
 * Parsing BSP splitting planes.
 * Returns 1 on success, 0 otherwise.
 */
static int bsp_map_parse_planes(const bsp_header_t *header, bsp_map_t *map)
{
	int i, num_planes;
	const bsp_lump_t *plane_lump;
	const bsp_plane_t *plane;
	const char *file_buffer;

	// Initialize plane array.
	plane_lump = &header->lumps[BSP_LUMP_PLANES];
	num_planes = (int)(plane_lump->length / sizeof(bsp_plane_t));
	if (!bsp_map_initialize_planes(map, num_planes)) {
		return 0;
	}

	// Fill out the planes.
	file_buffer = (const char*)header;
	plane = (const bsp_plane_t*)&file_buffer[plane_lump->offset];
	for (i = 0; i < num_planes; ++i, ++plane) {
		bsp_plane_t *destination;

		// Copy plane from file to structure.
		destination = &map->planes[i];
		vector3d_copy(&plane->normal, &destination->normal);
		destination->distance = plane->distance;
		destination->type = plane->type;
	}
	return 1;
}