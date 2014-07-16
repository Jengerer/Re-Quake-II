#ifndef _MATH_COMMON_H_
#define _MATH_COMMON_H_

#include <math.h>

#define PI 3.14159265f

// Convert angle value from degrees to radians.
float degrees_to_radians(float angle);

// Convert angle from radians to degrees.
float radians_to_degrees(float angle);

// Clamp an angle value within the range -180.0 to 180.0.
void clamp_angle(float *out);

#endif // _MATH_COMMON_H_