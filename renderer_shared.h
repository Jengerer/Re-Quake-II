#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"

// Generic shader types.
enum RendererShaderType
{
	VertexShader,
	FragmentShader
};

// Generic renderer variable types.
enum RendererVariableType
{
	FloatType,
	Vector2Type,
	Vector3Type,
	Vector4Type,
	Matrix2x2Type,
	Matrix3x3Type,
	Matrix4x4Type
};

// Shader attribute definition.
class RendererShaderAttribute
{

public:

	RendererShaderAttribute(const char *name, RendererVariableType type);

	// Parameter retrieval.
	const char *GetName() const { return name; }
	RendererVariableType GetType() const { return type; }

private:

	const char *name;
	RendererVariableType type;

};

// Schema for a shader program.
class RendererShaderSchema
{
};

// Single compiled shader.
class RendererShader
{
};

// Full shader program.
class RendererProgram
{
};

// Shader uniform variable.
class RendererUniform
{

public:

	// Value setting functions.
	// Set value as 3-D vector.
	virtual void SetVector3(const Vector3 *vector) = 0;
	// Set value as 4-D vector.
	virtual void SetVector4(const Vector4 *vector) = 0;
	// Set value as 3x3 matrix.
	virtual void SetMatrix3x3(const Matrix3x3 *matrix) = 0;
	// Set value as 4x4 matrix.
	virtual void SetMatrix4x4(const Matrix4x4 *matrix) = 0;

};

// TWo dimensional texture.
class RendererTexture
{
};

// Component to render in a scene.
class RendererModel
{
};