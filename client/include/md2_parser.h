#pragma once

#include "entity_model.h"
#include <file.h>
#include <stdint.h>
#include <vector3.h>

namespace MD2
{

	// MD2 file header.
	struct Header
	{
		// Header verification.
		int32_t magicNumber;
		int32_t version;

		// Texture sizes.
		int32_t skinWidth;
		int32_t skinHeight;

		// Byte size of vertices in each frame.
		int32_t frameSize;

		// Section sizes.
		int32_t skinCount;
		int32_t vertexCount;
		int32_t textureCoordinateCount;
		int32_t triangleCount;
		int32_t commandCount;
		int32_t frameCount;

		// Section offsets.
		int32_t skinsOffset;
		int32_t textureCoordinatesOffset;
		int32_t trianglesOffset;
		int32_t framesOffset;
		int32_t commandsOffset;
		int32_t endOffset;
	};

	// Vertex structure.
	struct Vertex
	{
		// 3-D compressed coordinate.
		unsigned char x;
		unsigned char y;
		unsigned char z;

		// Index into normal map/array.
		unsigned char normalIndex;
	};

	// Structure representing one frame of an animation.
	// After each MD2 frame structure follow the vertices for that frame.
	static const int FrameNameSize = 16;
	struct Frame
	{
		Vector3 scale;
		Vector3 offset;
		char name[FrameNameSize];
	};

	// Texture coordinate file structure.
	struct TextureCoordinate
	{
		int16_t s;
		int16_t t;
	};

	// Structure representing a triangle in the mesh.
	static const int VerticesPerTriangle = 3;
	struct Triangle
	{
		int16_t vertexIndices[VerticesPerTriangle];
		int16_t textureCoordinateIndices[VerticesPerTriangle];
	};

	// Class for loading an entity model from an MD2 file.
	class Parser
	{

	public:

		Parser();
		~Parser();

		// Parse a model from a file.
		// Assumes the output model is new.
		bool Load(const char *filename, EntityModel *out);

	private:

		// Verify header.
		bool VerifyHeader();

		// Load the frames.
		void LoadFrames();

		// Load the texture coordinates.
		void LoadTextureCoordinates();

	private:

		// Header magic number and version.
		static const int32_t MagicNumber = ('2' << 24) | ('P' << 16) | ('D' << 8) | 'I';
		static const int32_t Version = 8;

	private:

		// Model file data.
		FileData modelFile;
		const uint8_t *data;

		// Model we're filling in.
		EntityModel *out;

		// Pointers to parts of the model.
		const Header *header;

		// Triangles array.
		const Triangle *triangles;
		int32_t triangleCount;

	};

}
