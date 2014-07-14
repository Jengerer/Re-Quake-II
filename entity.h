#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "vector3d.h"

// Base world entity object.
typedef struct entity
{
	vector3d_t origin;
	vector3d_t angles;
} entity_t;

#endif // _ENTITY_H_