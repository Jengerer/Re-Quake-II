#pragma once

// Common math functions.
class MathCommon
{
public:

	// Angle functions.

	// Convert angle from degrees to radians.
	static float DegreesToRadians(float angle);
	// Convert angle from radians to degrees.
	static float RadiansToDegrees(float angle);
	// Clamp angle to the range [-180.0, 180.0f].
	static float ClampAngle(float angle);

	// Generic math functions.

	// Calculate square root of value.
	static float SquareRoot(float value);

	// Number rounding.

	// Round to nearest whole number.
	static float Round(float value);
	// Round to whole value at least as large as this one.
	static float RoundUp(float value);
	// Round to whole value at most as large as this one.
	static float RoundDown(float value);

	// Trigonometric functions (in degrees).

	// Sine function.
	static float Sine(float angle);
	// Cosine function.
	static float Cosine(float angle);
	// Arctangent with both X and Y parameters.
	static float ArcTangent(float y, float x);

public:

	// Math constants.

	static const float Pi;

};