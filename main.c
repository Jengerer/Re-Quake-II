#include <stdio.h>
#include "polygon3d.h"

#define NUM_VERTICES 5

int main(int argc, char *argv[]) {
	int i;
	polygon3d_t polygon;
	const vector3d_t *vertex;
	vector3d_t normal;

	// Allocate 5 vertices.
	if (!initialize_polygon(&polygon, NUM_VERTICES)) {
		printf("Polygon initialization failed.");
		return -1;
	}

	// Initialize to pentagonal shape.
	vector_set(&polygon.vertices[0], 0.0f, 1.0f, 0.0f);
	vector_set(&polygon.vertices[1], 0.5f, 0.5f, 0.0f);
	vector_set(&polygon.vertices[2], 0.5f, -0.5f, 0.0f);
	vector_set(&polygon.vertices[3], -0.5f, -0.5f, 0.0f);
	vector_set(&polygon.vertices[4], -0.5f, 0.5f, 0.0f);

	// Run through indices.
	for (i = 0; i < polygon.index_count; ++i) {
		vertex = get_polygon_indexed_vertex(&polygon, i);
		printf("(%f, %f, %f)\n", vertex->x, vertex->y, vertex->z);
	}
	destroy_polygon(&polygon);

	// Calculate normal.
	get_polygon_normal(&polygon, &normal);
	printf("(%f, %f, %f)\n", normal.x, normal.y, normal.z);
	return 0;
}
