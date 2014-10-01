#include "matrix4x4.h"
#include "math_common.h"

// Fill the matrix with identity values.
void Matrix4x4::Identity()
{
	for (int i = 0; i < Size; ++i) {
		for (int j = 0; j < Size; ++j) {
			matrixArray[i][j] = (i == j ? 1.0f : 0.0f);
		}
	}
}

// Get the transpose of a matrix.
void Matrix4x4::Transpose(const Matrix4x4 *matrix)
{
	// Copy diagonal.
	for (int i = 0; i < Size; ++i) {
		matrixArray[i][i] = matrix->matrixArray[i][i];
	}

	// Swap elements on opposide sides of the diagonal.
	for (int i = 0; i < Size; ++i) {
		for (int j = i + 1; j < Size; ++j) {
			// Keep temporary in case this is the operand.
			float copy = matrix->matrixArray[i][j];
			matrixArray[i][j] = matrixArray[j][i];
			matrixArray[j][i] = copy;
		}
	}
}

// Store result of product of two matrices into this matrix.
// This matrix cannot be one of the operands.
void Matrix4x4::Product(const Matrix4x4 *a, const Matrix4x4 *b)
{
	int i, j, k;

	// For each of our elements...
	for (int i = 0; i < Size; ++i) {
		for (int j = 0; j < Size; ++j) {
			// Element I, J is the dot product of row I of A and column J of B.
			float product = 0.0f;
			for (int k = 0; k < Size; ++k) {
				product += a->matrixArray[i][k] * b->matrixArray[k][j];
			}
			matrixArray[i][j] = product;
		}
	}
}

// Generate a matrix representation a translation transformation.
void Matrix4x4::Translation(const Vector3 *translation)
{
	matrixArray[0][0] = 1.0f;
	matrixArray[0][1] = 0.0f;
	matrixArray[0][2] = 0.0f;
	matrixArray[0][3] = translation->x;
	matrixArray[1][0] = 0.0f;
	matrixArray[1][1] = 1.0f;
	matrixArray[1][2] = 0.0f;
	matrixArray[1][3] = translation->y;
	matrixArray[2][0] = 0.0f;
	matrixArray[2][1] = 0.0f;
	matrixArray[2][2] = 1.0f;
	matrixArray[2][3] = translation->z;
	matrixArray[3][0] = 0.0f;
	matrixArray[3][1] = 0.0f;
	matrixArray[3][2] = 0.0f;
	matrixArray[3][3] = 1.0f;
}

// Generate a rotation matrix around the X axis.
void Matrix4x4::RotationX(float angle)
{
	float cosAngle;
	float sinAngle;

	// Calculate common values.
	cosAngle = MathCommon::Cosine(angle);
	sinAngle = MathCommon::Sine(angle);

	// Fill in matrix.
	matrixArray[0][0] = 1.0f;
	matrixArray[0][1] = 0.0f;
	matrixArray[0][2] = 0.0f;
	matrixArray[0][3] = 0.0f;
	matrixArray[1][0] = 0.0f;
	matrixArray[1][1] = cosAngle;
	matrixArray[1][2] = -sinAngle;
	matrixArray[1][3] = 0.0f;
	matrixArray[2][0] = 0.0f;
	matrixArray[2][1] = sinAngle;
	matrixArray[2][2] = cosAngle;
	matrixArray[2][3] = 0.0f;
	matrixArray[3][0] = 0.0f;
	matrixArray[3][1] = 0.0f;
	matrixArray[3][2] = 0.0f;
	matrixArray[3][3] = 1.0f;
}

// Generate a rotation matrix around the Y axis.
void Matrix4x4::RotationY(float angle)
{
	float cosAngle;
	float sinAngle;

	// Calculate common values.
	cosAngle = MathCommon::Cosine(angle);
	sinAngle = MathCommon::Sine(angle);

	// Fill in matrix.
	matrixArray[0][0] = cosAngle;
	matrixArray[0][1] = 0.0f;
	matrixArray[0][2] = sinAngle;
	matrixArray[0][3] = 0.0f;
	matrixArray[1][0] = 0.0f;
	matrixArray[1][1] = 1.0f;
	matrixArray[1][2] = 0.0f;
	matrixArray[1][3] = 0.0f;
	matrixArray[2][0] = -sinAngle;
	matrixArray[2][1] = 0.0f;
	matrixArray[2][2] = cosAngle;
	matrixArray[2][3] = 0.0f;
	matrixArray[3][0] = 0.0f;
	matrixArray[3][1] = 0.0f;
	matrixArray[3][2] = 0.0f;
	matrixArray[3][3] = 1.0f;
}

// Generate a rotation matrix around the Z axis.
void Matrix4x4::RotationZ(float angle)
{
	float cosAngle;
	float sinAngle;

	// Calculate common values.
	cosAngle = MathCommon::Cosine(angle);
	sinAngle = MathCommon::Sine(angle);

	// Fill in matrix.
	matrixArray[0][0] = cosAngle;
	matrixArray[0][1] = -sinAngle;
	matrixArray[0][2] = 0.0f;
	matrixArray[0][3] = 0.0f;
	matrixArray[1][0] = sinAngle;
	matrixArray[1][1] = cosAngle;
	matrixArray[1][2] = 0.0f;
	matrixArray[1][3] = 0.0f;
	matrixArray[2][0] = 0.0f;
	matrixArray[2][1] = 0.0f;
	matrixArray[2][2] = 1.0f;
	matrixArray[2][3] = 0.0f;
	matrixArray[3][0] = 0.0f;
	matrixArray[3][1] = 0.0f;
	matrixArray[3][2] = 0.0f;
	matrixArray[3][3] = 1.0f;
}

// Get full Euler angle rotation transformation in order YXZ (object space).
 void Matrix4x4::RotationEuler(const Vector3 *angles)
 {
	 Matrix4x4 rotationX, rotationY, rotationZ;
	 Matrix4x4 rotationZX;

	 // Need to apply matrices in reverse order.
	 rotationZ.RotationZ(angles->z);
	 rotationX.RotationX(angles->x);
	 rotationY.RotationY(angles->y);
	 rotationZX.Product(&rotationZ, &rotationX);
	 Product(&rotationY, &rotationZX);
 }

 // Generate a perspective projection matrix by frustrum parameters.
 void Matrix4x4::PerspectiveProjectionFrustum(
	 float left,
	 float right,
	 float top,
	 float bottom,
	 float zNear,
	 float zFar)
 {
	float doubleZNear;
	float inverseRightLeftDiff, inverseTopBottomDiff, inverseFarNearDiff;

	// Get common values.
	doubleZNear = 2.0f * zNear;
	inverseRightLeftDiff = 1.0f / (right - left);
	inverseTopBottomDiff = 1.0f / (top - bottom);
	inverseFarNearDiff = 1.0f / (zFar - zNear);

	// Fill in the matrix.
	matrixArray[0][0] = doubleZNear * inverseRightLeftDiff;
	matrixArray[0][1] = 0.0f;
	matrixArray[0][2] = (right + left) * inverseRightLeftDiff;
	matrixArray[0][3] = 0.0f;
	matrixArray[1][0] = 0.0f;
	matrixArray[1][1] = doubleZNear * inverseTopBottomDiff;
	matrixArray[1][2] = (top + bottom) * inverseTopBottomDiff;
	matrixArray[1][3] = 0.0f;
	matrixArray[2][0] = 0.0f;
	matrixArray[2][1] = 0.0f;
	matrixArray[2][2] = (zFar + zNear) * inverseFarNearDiff;
	matrixArray[2][3] = -(doubleZNear * zFar) * inverseFarNearDiff;
	matrixArray[3][0] = 0.0f;
	matrixArray[3][1] = 0.0f;
	matrixArray[3][2] = 1.0f;
	matrixArray[3][3] = 0.0f;
 }

// Generate a perspective projection matrix.
void Matrix4x4::PerspectiveProjection(
	float aspectRatio,
	float fieldOfView,
	float zNear,
	float zFar)
{
	float xMax, yMax;

	// Calculate common values.
	yMax = zNear * MathCommon::Tangent(fieldOfView);
	xMax = yMax * aspectRatio;

	// Fill out matrix.
	PerspectiveProjectionFrustum(-xMax, xMax, yMax, -yMax, zNear, zFar);
}

// Multiply a matrix by a vector.
void Matrix4x4::Transform(const Vector4 *vector, Vector4 *out)
{
	out->x = (vector->x * matrixArray[0][0]) + (vector->y * matrixArray[0][1]) + (vector->z * matrixArray[0][2]) + (vector->w * matrixArray[0][3]);
	out->y = (vector->x * matrixArray[1][0]) + (vector->y * matrixArray[1][1]) + (vector->z * matrixArray[1][2]) + (vector->w * matrixArray[1][3]);
	out->z = (vector->x * matrixArray[2][0]) + (vector->y * matrixArray[2][1]) + (vector->z * matrixArray[2][2]) + (vector->w * matrixArray[2][3]);
	out->w = (vector->x * matrixArray[3][0]) + (vector->y * matrixArray[3][1]) + (vector->z * matrixArray[3][2]) + (vector->w * matrixArray[3][3]);
}
