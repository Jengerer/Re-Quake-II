#ifndef _MAP_H_
#define _MAP_H_

/*
 * A map represented by a set of world planes.
 */
typedef struct map
{
	plane_t *planes;
	int plane_count;
};

#endif // _MAP_H_
