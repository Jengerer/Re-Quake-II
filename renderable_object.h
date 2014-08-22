#ifndef _RENDERABLE_OBJECT_H_
#define _RENDERABLE_OBJECT_H_

#include "renderer.h"
#include "world.h"
#include "object.h"

/* Object for drawing a polygon corresponding to a physics object. */
typedef struct renderable_object
{
	renderer_model_t model;
	const object_t *object;

	// Linked list nodes.
	struct renderable_object *next;
	struct renderable_object *prev;
} renderable_object_t;

// Object initialization and clean-up functions.
void renderable_object_null(renderable_object_t *renderable);
int renderable_object_initialize(
	renderable_object_t *renderable,
	const object_t *object);
void renderable_object_destroy(renderable_object_t *object);

// Renderer resource handling and rendering.
int renderable_object_load_resources(
	renderable_object_t *renderable,
	const renderer_t *renderer,
	renderer_shader_schema_t schema);
void renderable_object_free_resources(
	renderable_object_t *renderable,
	const renderer_t *renderer);
void renderable_object_render(
	const renderable_object_t *renderable,
	const renderer_t *renderer,
	renderer_uniform_t object_transform,
	renderer_shader_schema_t schema);

#endif // _RENDERABLE_OBJECT_H_