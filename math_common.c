#include "math_common.h"
#include <math.h>

/* Convert from degrees to radians. */
float degrees_to_radians(float angle)
{
	return angle * (180.0f / PI);
}

/* Convert from radians to degrees. */
float radians_to_degrees(float angle)
{
	return angle * (PI / 180.0f);
}
