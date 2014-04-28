#ifndef _AABB_ENTITY_H_
#define _AABB_ENTITY_H_

#include "aabb.h"
#include "entity.h"

// Object for world entities that have bounds.
typedef struct aabb_entity
{
	entity_t base;
	aabb_t aabb;
} aabb_entity_t;

#endif // _AABB_ENTITY_H_