#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "world_object.h"

// Base world entity object.
typedef struct entity
{
	world_object_t *object;
} entity_t;

#endif // _ENTITY_H_