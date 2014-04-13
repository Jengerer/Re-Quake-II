#include "engine.h"
#include "opengl_renderer.h"
#include "map.h"
#include <stdio.h>

// Application constants.
#define APPLICATION_TITLE "Engine Test"
#define APPLICATION_WIDTH 1024
#define APPLICATION_HEIGHT 768

int main(int argc, char *argv[])
{
	map_t map;
	aabb_t aabb;
	vector3d_t end;
	trace_t trace;
	mesh_t *mesh;
	indexed_mesh_t *indexed_mesh;
	polygon_t *polygon;
	plane_t *plane;

	// Engine state.
	engine_t engine;
	engine_configuration_t *config = &engine.config;

	// Create configuration for engine.
	config->title = APPLICATION_TITLE;
	config->width = APPLICATION_WIDTH;
	config->height = APPLICATION_HEIGHT;

	// Prepare OpenGL renderer.
	initialize_opengl_interface(&engine.renderer);

	// Initialize engine.
	null_engine(&engine);
	if (!initialize_engine(&engine)) {
		// Destroy partially initialized state.
		destroy_engine(&engine);
		return -1;
	}

	// Create test map.
	if (!initialize_map(&map, 1)) {
		printf("Failed to initialize map.\n");
		return -1;
	}
	polygon = &map.polygons[0];
	indexed_mesh = &polygon->indexed_mesh;
	mesh = &indexed_mesh->mesh;
	if (!initialize_polygon(polygon, 6)) {
		printf("Failed to initialize polygon.\n");
		return -1;
	}
	vector_set(&mesh->vertices[0], -1.0f, -1.0f, -1.0f);
	vector_set(&mesh->vertices[1], -1.0, 0.0f, -1.0f);
	vector_set(&mesh->vertices[2], 0.0f, -1.0f, -1.0f);
	vector_set(&mesh->vertices[3], 1.0f, 0.5f, -1.0f);
	vector_set(&mesh->vertices[4], 1.0f, 1.0f, -1.0f);
	vector_set(&mesh->vertices[5], 0.5f, 1.0f, -1.0f);
	calculate_polygon_plane(polygon);
	plane = &polygon->plane;

	// Test printing polygon.
	printf("Normal: (%f, %f, %f), Distance: %f\n", plane->normal.x, plane->normal.y, plane->normal.z, plane->distance);

	// Create test AABB.
	vector_set(&aabb.mins, -0.25f, -0.25f, -0.25f);
	vector_set(&aabb.maxs, 0.25f, 0.25f, 0.25f);
	vector_set(&aabb.position, 0.0f, 0.0f, 0.0f);
	
	// Test collision behind the plane.
	vector_set(&end, 0.0f, 0.0f, -10.0f);
	trace_aabb(&map, &aabb, &aabb.position, &end, &trace);

	// Run engine and shut down.
	run_engine(&engine);
	destroy_engine(&engine);

	return 0;
}

