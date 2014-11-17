#pragma once

#include "buffer.h"
#include "buffer_schema.h"
#include "index_buffer.h"
#include "material.h"

namespace Renderer
{

	// Variable storage types.
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
	enum PrimitiveType
	{
		Triangles,
		TriangleFan,
		TriangleStrip,
	};

	// Material attribute type.
	enum AttributeType
	{
		PositionType,
		NormalType,
		TextureCoordinateType
	};

	// Vertex buffer representation of shader attributes.
	class Attribute
	{

	public:

		Attribute(const char *name, AttributeType attributeType, DataType dataType);
		inline const char *GetName() const { return name; }
		inline DataType GetType() const { return type; }

	private:

		const char *name;
		AttributeType attributeType;
		DataType dataType;

	};

}
