#include "opengl_shader_attribute.h"
#include "error_stack.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace OpenGL
{

	// Shader attribute parameter constructor.
	ShaderAttribute::ShaderAttribute(
		GLint location,
		GLchar *offset,
		Renderer::DataType type) : location(location), offset(offset)
	{
		elementType = GetElementType(type);
		elementCount = GetElementCount(type);
	}

	// Activate this shader attribute for rendering.
	void ShaderAttribute::Activate() const
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,
			elementCount,
			elementType,
			GL_FALSE,
			0,
			offset);
	}

	// Deactivate this shader attribute from rendering.
	void ShaderAttribute::Deactivate() const
	{
		glDisableVertexAttribArray(location);
	}

	// Get size of base element of a data type.
	GLsizei ShaderAttribute::GetDataTypeSize(Renderer::DataType type)
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
			return sizeof(float);
		case Renderer::Vector2Type:
			return sizeof(Vector2);
		case Renderer::Vector3Type:
			return sizeof(Vector3);
		case Renderer::Vector4Type:
			return sizeof(Vector4);
		case Renderer::Matrix3x3Type:
			return sizeof(Matrix3x3);
		case Renderer::Matrix4x4Type:
			return sizeof(Matrix4x4);
		default:
			ErrorStack::Log("Unknown type declared for renderer data type.");
			return 0;
		}
	}

	// Get the GL enumeration type for the base attribute element type.
	GLenum ShaderAttribute::GetElementType(Renderer::DataType type)
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
	GLsizei ShaderAttribute::GetElementCount(Renderer::DataType type)
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