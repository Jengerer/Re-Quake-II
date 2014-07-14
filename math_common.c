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

/* Clamp angle to range of -180.0 to 180.0. */
void clamp_angle(float *out)
{
	// Subtract the multiple of 360 degrees that we're over.
	// Use integer rounding so that -1.5 rounds to -1.0.
	float angle = *out;
	*out = angle - (360.0f * roundf(angle / 360.0f));
}