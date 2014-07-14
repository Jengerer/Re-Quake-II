#include "platformer.h"
#include "player_move.h"
#include "math_common.h"
#include <stdio.h>
#include <stdlib.h>

// Game parameters.
#define PLATFORMER_NAME "Platformer Test"

// Rendering parameters.
#define NUM_PERSPECTIVE_SHADER_ATTRIBUTES 2
#define VERTEX_SHADER_FILE "engine.vert"
#define FRAGMENT_SHADER_FILE "engine.frag"

// Global platformer context.
static platformer_context_t platformer;

// Renderer shader schemas.
const renderer_shader_attribute_t mesh_attributes[NUM_PERSPECTIVE_SHADER_ATTRIBUTES] = {
	{"in_vertex", VARIABLE_VERTEX_3D},
	{"in_texture", VARIABLE_VERTEX_2D}
};

// Private functions.
int initialize_shaders(renderer_t *renderer);

/*
 * Null platformer context for safe destruction.
 */
void null_platformer_context(platformer_context_t *context)
{
	null_map(&context->map);
	renderer_null_shader(&context->vertex_shader);
	renderer_null_shader(&context->fragment_shader);
	renderer_null_program(&context->program);
	renderer_null_shader_schema(&context->schema);
	renderer_null_uniform(&context->projection);
}

/*
 * Fill out game interface with platformer functions.
 */
void initialize_platformer_interface(game_t *game)
{
	// Null context here.
	null_platformer_context(&platformer);

	// Initialize interface functions.
	game->get_name = &get_platformer_name;
	game->initialize = &initialize_platformer;
	game->destroy = &destroy_platformer;
	game->load_resources = &load_platformer_resources;
	game->free_resources = &free_platformer_resources;
	game->render = &render_platformer;
	game->handle_keyboard = &handle_platformer_keyboard;
}

/*
 * Return name of the platformer game.
 */
const char* get_platformer_name(void)
{
	return PLATFORMER_NAME;
}

/*
 * Initialize platformer game.
 * Returns 1 on success, 0 otherwise.
 * Context is filled out as soon as there's something to clean.
 */
int initialize_platformer(void)
{
	player_t *player;
	map_t *map;
	polygon_t *polygon;
	mesh_t *mesh;
	indexed_mesh_t *indexed_mesh;

	// Create player.
	player = &platformer.player;
	initialize_player(player);

	// Initialize the map.
	map = &platformer.map;
	if (!initialize_map(map, 1)) {
		printf("Failed to initialize map.\n");
		return 0;
	}
	polygon = &map->polygons[0];
	indexed_mesh = &polygon->indexed_mesh;
	mesh = &indexed_mesh->mesh;
	if (!initialize_polygon(polygon, 4)) {
		printf("Failed to initialize polygon.\n");
		return 0;
	}
	vector3d_set(&mesh->vertices[0].position, -1.0f, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[1].position, 0.0, 1.0f, 0.0f);
	vector3d_set(&mesh->vertices[2].position, 0.0f, 0.0f, 0.0f);
	vector3d_set(&mesh->vertices[3].position, -1.0f, 0.0f, 0.0f);
	vector2d_set(&mesh->vertices[0].texture, 0.0f, 0.0f);
	vector2d_set(&mesh->vertices[1].texture, 1.0f, 0.0f);
	vector2d_set(&mesh->vertices[2].texture, 1.0f, 1.0f);
	vector2d_set(&mesh->vertices[3].texture, 0.0f, 1.0f);
	calculate_polygon_plane(polygon);
	return 1;
}

/*
 * Destroy the platformer context.
 */
void destroy_platformer(void)
{
	// Destroy the map.
	destroy_map(&platformer.map);
}

/*
 * Load all base resources required by platformer.
 */
int load_platformer_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;
	indexed_mesh_t *indexed_mesh;
	mesh_t *mesh;
	matrix4x4_t perspective_matrix;
	vector3d_t test3;
	vector4d_t test, test2;

	// Initialize shaders.
	if (!initialize_shaders(renderer)) {
		return 0;
	}

	// Set up shader for models.
	renderer->set_program(platformer.program);

	// Create models for the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		indexed_mesh = &polygon->indexed_mesh;
		mesh = &indexed_mesh->mesh;
		if (!renderer->create_indexed_model(mesh->vertices,
			mesh->num_vertices,
			indexed_mesh->indices,
			indexed_mesh->num_indices,
			platformer.schema,
			&polygon->model))
		{
			return 0;
		}
	}

	// Get the location to the transform and projection matrix.
	if (!renderer->get_uniform(platformer.program, "object", &platformer.object)) {
		return 0;
	}
	if (!renderer->get_uniform(platformer.program, "view", &platformer.view)) {
		return 0;
	}
	if (!renderer->get_uniform(platformer.program, "projection", &platformer.projection)) {
		return 0;
	}

	// Generate projection matrix.
	matrix4x4_perspective(4.0f / 3.0f, 45.0f, 1.0f, 1000.0f, &perspective_matrix);
	test.x = -1.0f;
	test.y = -1.0f;
	test.z = 4.0f;
	test.w = 1.0f;
	matrix4x4_transform(&perspective_matrix, &test, &test2);
	vector4d_to_vector3d(&test2, &test3);
	renderer->set_uniform_matrix4x4(platformer.projection, &perspective_matrix);
	return 1;
}

/*
 * Free base resources for platformer.
 */
void free_platformer_resources(renderer_t *renderer)
{
	int i;
	map_t *map;
	polygon_t *polygon;

	// Destroy models for the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->destroy_model(&polygon->model);
	}

	// Release uniform variable handles.
	renderer->destroy_uniform(&platformer.object);
	renderer->destroy_uniform(&platformer.view);
	renderer->destroy_uniform(&platformer.projection);

	// Destroy schema.
	renderer->destroy_shader_schema(&platformer.schema);

	// Destroy program and shaders.
	renderer->destroy_shader(&platformer.vertex_shader, platformer.program);
	renderer->destroy_shader(&platformer.fragment_shader, platformer.program);
	renderer->destroy_program(&platformer.program);
}

/*
 * Platformer rendering function.
 */
int render_platformer(renderer_t *renderer)
{
	int i;
	camera_t *camera;
	map_t *map;
	polygon_t *polygon;
	matrix4x4_t translation;
	matrix4x4_t view;

	// Clear the scene.
	renderer->clear_scene();
	renderer->set_program(platformer.program);

	// Set up camera transform.
	camera = &platformer.camera;
	camera_world_to_view_transform(camera, &view);
	renderer->set_uniform_matrix4x4(platformer.view, &view);

	// Move player.
	platformer.player.entity.origin.z = 4.0f;
	matrix4x4_translation(&platformer.player.entity.origin, &translation);
	renderer->set_uniform_matrix4x4(platformer.object, &translation);

	// Render the map polygons.
	map = &platformer.map;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		renderer->draw_model(polygon->model, platformer.schema);
	}
	return 1;
}

/*
 * Handle keyboard input for platjformer.
 */
void handle_platformer_keyboard(keyboard_manager_t *keyboard)
{
	camera_t *camera;
	player_t *player;
	player_move_t *move;
	entity_t *camera_entity, *player_entity;
	vector3d_t *camera_position, *camera_angles;
	vector3d_t *move_velocity, *angular_velocity;
	vector3d_t view_forward, view_right, view_velocity;

	camera = &platformer.camera;
	player = &platformer.player;
	player_entity = &player->entity;
	camera_entity = &camera->entity;
	move = &player->move;
	camera_position = &camera_entity->origin;
	camera_angles = &camera_entity->angles;
	move_velocity = &move->move_direction;
	angular_velocity = &move->turn_angles;
	
	// Update player movement.
	handle_player_move(keyboard, move);

	// Turn camera.
	if (vector3d_magnitude(&move->turn_angles) != 0.0f) {
		vector3d_add(camera_angles, angular_velocity, camera_angles);
		clamp_angle(&camera_angles->x);
		clamp_angle(&camera_angles->y);
	}

	// Move the camera by the command.
	if (vector3d_magnitude(move_velocity) != 0.0f) {
		// Get the direction vectors for camera view.
		angles_to_vector3d(camera_angles, &view_forward, &view_right, NULL);
		vector3d_scale(&view_forward, move_velocity->z, &view_velocity);
		vector3d_scalar_add(&view_velocity,move_velocity->x, &view_right, &view_velocity);
		vector3d_add(camera_position, &view_velocity, camera_position);
	}
}

/*
 * Initialize the platformer's shaders for rendering.
 */
int initialize_shaders(renderer_t *renderer)
{
	renderer_shader_t vertex_shader;
	renderer_shader_t fragment_shader;
	renderer_program_t program;
	renderer_shader_schema_t schema;

	// Load the perspective shader.
	if (!renderer->create_program(&program)) {
		return 0;
	}
	platformer.program = program;

	// Create vertex and fragment shaders.
	if (!renderer->create_shader(VERTEX_SHADER_FILE, VERTEX_SHADER, &vertex_shader)) {
		return 0;
	}
	platformer.vertex_shader = vertex_shader;
	if (!renderer->create_shader(FRAGMENT_SHADER_FILE, FRAGMENT_SHADER, &fragment_shader)) {
		return 0;
	}
	platformer.fragment_shader = fragment_shader;

	// Link both shaders.
	renderer->link_shader(vertex_shader, program);
	renderer->link_shader(fragment_shader, program);
	if (!renderer->compile_program(program)) {
		return 0;
	}

	// Get the renderer schema from the program.
	if (!renderer->create_shader_schema(program, mesh_attributes, NUM_PERSPECTIVE_SHADER_ATTRIBUTES, &schema)) {
		return 0;
	}
	platformer.schema = schema;
	
	return 1;
}
