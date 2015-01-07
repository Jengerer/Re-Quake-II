#pragma once

#include "common_define.h"

// Common math functions.
class CommonLibrary MathCommon
{

public:

	// Convert angle from degrees to radians.
	static float DegreesToRadians(float angle);

	// Convert angle from radians to degrees.
	static float RadiansToDegrees(float angle);

	// Clamp angle to the range [-180.0, 180.0f].
	static float ClampAngle(float angle);

	// Calculate square root of value.
	static float SquareRoot(float value);

	// Round to nearest whole number.
	static float Round(float value);

	// Round to whole value at least as large as this one.
	static float RoundUp(float value);

	// Round to whole value at most as large as this one.
	static float RoundDown(float value);

	// Sine function.
	static float Sine(float angle);

	// Cosine function.
	static float Cosine(float angle);

	// Tangent function.
	static float Tangent(float angle);

	// Arctangent with both X and Y parameters.
	static float ArcTangent(float y, float x);

public:

	static const float Pi;

};