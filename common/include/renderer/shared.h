#pragma once

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
		Polygon,
	};

	// Material attribute type.
	enum AttributeType
	{
		PositionType,
		NormalType,
		TextureCoordinateType
	};

}
