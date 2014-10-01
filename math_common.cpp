#include "math_common.h"
#include <math.h>

// Math constants.
const float MathCommon::Pi = 3.14159265f;

// Convert from degrees to radians.
float MathCommon::DegreesToRadians(float angle)
{
	return angle * (Pi / 180.0f);
}

// Convert from radians to degrees.
float MathCommon::RadiansToDegrees(float angle)
{
	return angle * (180.0f / Pi);
}

// Clamp angle to range of -180.0 to 180.0.
float MathCommon::ClampAngle(float angle)
{
	// Subtract the multiple of 360 degrees that we're over.
	// Use integer rounding so that -1.5 rounds to -1.0.
	return angle - (360.0f * Round(angle / 360.0f));
}

// Calculate square root.
float MathCommon::SquareRoot(float value)
{
	return sqrtf(value);
}

// Calculate rounded floating point value.
float MathCommon::Round(float value)
{
	return roundf(value);
}

// Calculate ceiling rounded floating point value.
float MathCommon::RoundUp(float value)
{
	return ceilf(value);
}

// Calculate floor rounded floating point value.
float MathCommon::RoundDown(float value)
{
	return floorf(value);
}
