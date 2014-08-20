#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "object.h"

// Base world entity object.
typedef struct entity
{
	object_t *object;
} entity_t;

#endif // _ENTITY_H_