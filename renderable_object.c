#include "renderable_object.h"
#include "memory_manager.h"

/* Null a renderable object for safe deletion. */
void renderable_object_null(renderable_object_t *renderable)
{
	renderer_null_model(&renderable->model);
	renderable->object = NULL;
}

/* Initialize a renderable object with its physics object. */
int renderable_object_initialize(
	renderable_object_t *renderable,
	const object_t *object)
{
	renderable->object = object;
	return 1;
}

/* Destroy a renderable object. */
void renderable_object_destroy(renderable_object_t *object)
{
	(void)object;
}

/* Load resources for a renderable object. */
int renderable_object_load_resources(
	renderable_object_t *renderable,
	const renderer_t *renderer,
	renderer_shader_schema_t schema)
{
	renderer_model_t *model;
	const object_t *object;
	const polygon_t *polygon;
	const indexed_mesh_t *indexed_mesh;
	const mesh_t *mesh;

	// Create a model from the object's mesh.
	model = &renderable->model;
	object = renderable->object;
	polygon = &object->polygon;
	indexed_mesh = &polygon->indexed_mesh;
	mesh = &indexed_mesh->mesh;
	if (!renderer->create_indexed_model(
		mesh->vertices,
		mesh->num_vertices,
		indexed_mesh->indices,
		indexed_mesh->num_indices,
		schema,
		model))
	{
		return 0;
	}

	return 1;
}

/* Destroy resources for a renderable object. */
void renderable_object_free_resources(
	renderable_object_t *renderable,
	const renderer_t *renderer)
{
	// Free the model.
	renderer->destroy_model(&renderable->model);
}

/* Draw the model in the world. */
void renderable_object_render(
	const renderable_object_t *renderable,
	const renderer_t *renderer,
	renderer_uniform_t object_transform,
	renderer_shader_schema_t schema)
{
	matrix4x4_t translation;
	const object_t *object;

	// Set up object transform.
	object = renderable->object;
	matrix4x4_translation(&object->origin, &translation);
	renderer->set_uniform_matrix4x4(object_transform, &translation);

	// Draw the model.
	renderer->draw_model(renderable->model, schema);
}
