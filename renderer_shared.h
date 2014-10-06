#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"

namespace Renderer
{

	// Generic shader types.
	enum ShaderType
	{
		VertexShader,
		FragmentShader
	};

	// Generic renderer variable types.
	enum VariableType
	{
		FloatType,
		Vector2Type,
		Vector3Type,
		Vector4Type,
		Matrix2x2Type,
		Matrix3x3Type,
		Matrix4x4Type
	};

	// Renderer model type.
	enum ModelType
	{
		InvalidModel,
		TriangleFanModel,
		TriangleStripModel
	};

	// Shader attribute definition.
	class Attribute
	{

	public:

		Attribute(const char *name, VariableType type);
		inline const char *GetName() const;
		inline VariableType GetType() const;

	private:

		const char *name;
		VariableType type;

	};

	// Single compiled shader.
	class Shader
	{
	};

	// Full shader program.
	class Program
	{
	};

	// Schema for a shader program.
	class ShaderSchema
	{
	};

	// Shader uniform variable.
	class Uniform
	{
	};

	// Two dimensional texture.
	class Texture
	{
	};

	// Renderable component made of sequential vertices.
	class Model
	{
	};

	// Renderable component made of vertices and sequential indices that index them.
	class IndexedModel
	{
	};

	const char *Attribute::GetName() const
	{
		return name;
	}

	VariableType Attribute::GetType() const
	{
		return type;
	}

}