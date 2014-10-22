#pragma once

namespace Renderer
{

	// Generic shader types.
	enum ShaderType
	{
		VertexShader,
		FragmentShader,
	};

	// Generic renderer variable types.
	enum DataType
	{
		ByteType,
		UnsignedByteType,
		ShortType,
		UnsignedShortType,
		IntType,
		UnsignedIntType,
		FloatType,
		Vector2Type,
		Vector3Type,
		Vector4Type,
		Matrix2x2Type,
		Matrix3x3Type,
		Matrix4x4Type,
	};

	// Geometry rendering type.
	enum GeometryType
	{
		Triangles,
		TriangleFan,
		TriangleStrip,
	};

	// Vertex buffer representation of shader attributes.
	class Attribute
	{

	public:

		Attribute(const char *name, DataType type);
		inline const char *GetName() const { return name; }
		inline DataType GetType() const { return type; }

	private:

		const char *name;
		DataType type;

	};

	// Single compiled shader.
	class Shader
	{
	};

	// Full shader program.
	class Program
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

	// Generic renderer buffer through which data is passed to shader.
	class Buffer
	{
	};

	// Generic index buffer for mapping to a set of vertices.
	class IndexBuffer
	{
	};

	// Schema for mapping buffer data to shader program attributes.
	class BufferSchema
	{
	};

}
