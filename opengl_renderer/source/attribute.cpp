#include "attribute.h"
#include <error_stack.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix3x3.h>
#include <matrix4x4.h>

namespace OpenGL
{

	// Shader attribute parameter constructor.
	Attribute::Attribute() : location(-1)
	{
	}

	// Set the parameters for this attribute.
	int Attribute::SetParameters(
		GLint location,
		GLchar *offset,
		Renderer::DataType type)
	{
		this->location = location;
		this->offset = offset;
		elementType = GetElementType(type);
		elementCount = GetElementCount(type);

		// Increment offset by this attribute size.
		return (elementCount * GetElementSize(type));
	}

	// Activate this shader attribute for rendering.
	void Attribute::Activate(int stride) const
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,
			elementCount,
			elementType,
			GL_FALSE,
			static_cast<GLsizei>(stride),
			offset);
	}

	// Deactivate this shader attribute from rendering.
	void Attribute::Deactivate() const
	{
		glDisableVertexAttribArray(location);
	}

	// Get size of base element of a data type.
	GLsizei Attribute::GetElementSize(Renderer::DataType type)
	{
		switch (type) {
		case Renderer::ByteType:
		case Renderer::UnsignedByteType:
			return sizeof(char);
		case Renderer::ShortType:
		case Renderer::UnsignedShortType:
			return sizeof(short);
		case Renderer::IntType:
		case Renderer::UnsignedIntType:
			return sizeof(int);
		case Renderer::FloatType:
		case Renderer::Vector2Type:
		case Renderer::Vector3Type:
		case Renderer::Vector4Type:
		case Renderer::Matrix3x3Type:
		case Renderer::Matrix4x4Type:
			return sizeof(float);
		default:
			ErrorStack::Log("Unknown type declared for renderer data type.");
			return 0;
		}
	}

	// Get the GL enumeration type for the base attribute element type.
	GLenum Attribute::GetElementType(Renderer::DataType type)
	{
		switch (type) {
		case Renderer::ByteType:
			return GL_BYTE;
		case Renderer::UnsignedByteType:
			return GL_UNSIGNED_BYTE;
		case Renderer::ShortType:
			return GL_SHORT;
		case Renderer::UnsignedShortType:
			return GL_UNSIGNED_SHORT;
		case Renderer::IntType:
			return GL_INT;
		case Renderer::UnsignedIntType:
			return GL_UNSIGNED_INT;
		case Renderer::FloatType:
		case Renderer::Vector2Type:
		case Renderer::Vector3Type:
		case Renderer::Vector4Type:
		case Renderer::Matrix2x2Type:
		case Renderer::Matrix3x3Type:
		case Renderer::Matrix4x4Type:
			return GL_FLOAT;
		default:
			ErrorStack::Log("Unknown type declared for renderer data type.");
			return 0;
		}
	}

	// Get the number of base elements in the given data type.
	GLsizei Attribute::GetElementCount(Renderer::DataType type)
	{
		switch (type) {
		case Renderer::Vector2Type:
			return 2;
		case Renderer::Vector3Type:
			return 3;
		case Renderer::Vector4Type:
			return 4;
		case Renderer::Matrix2x2Type:
			return (2 * 2);
		case Renderer::Matrix3x3Type:
			return (3 * 3);
		case Renderer::Matrix4x4Type:
			return (4 * 4);
		default:
			// Can safely assume everything else is single.
			return 1;
		}
	}

}