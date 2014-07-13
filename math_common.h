#ifndef _MATH_COMMON_H_
#define _MATH_COMMON_H_

#include <math.h>

#define PI 3.14159265f

// Convert angle value from degrees to radians.
float degrees_to_radians(float angle)
{
	return angle * (180.0f / PI);
}

// Convert angle from radians to degrees.
float radians_to_degrees(float angle)
{
	return angle * (PI / 180.0f);
}

#endif // _MATH_COMMON_H_