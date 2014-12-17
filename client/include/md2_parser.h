#pragma once

#include "entity_model.h"
#include <file.h>
#include <stdint.h>
#include <vector3.h>

// MD2 file header.
struct MD2Header
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
struct MD2Vertex
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
struct MD2Frame
{
	Vector3 scale;
	Vector3 offset;
	char name[FrameNameSize];
};

// Command structure.
struct MD2Command
{
	int32_t vertexCount;
};

// Command vertex.
struct MD2CommandPacket
{
	float textureS;
	float textureT;
	int32_t vertexIndex;
};
const static int PacketIntegerCount = sizeof(MD2CommandPacket) / sizeof(int32_t);

// Class for loading an entity model from an MD2 file.
class MD2Parser
{

public:

	MD2Parser();
	~MD2Parser();

	// Parse a model from a file.
	// Assumes the output model is new.
	bool Load(const uint8_t *modelData, EntityModel *out);

private:

	// Verify header.
	bool VerifyHeader();

	// Load the frames.
	void LoadFrames();
	
	// Load commands into entity model.
	bool LoadCommands();

private:

	// Header magic number and version.
	static const int32_t MagicNumber = ('2' << 24) | ('P' << 16) | ('D' << 8) | 'I';
	static const int32_t Version = 8;

private:

	// Raw data.
	const uint8_t *data;

	// Model we're filling in.
	EntityModel *out;

	// Pointers to parts of the model.
	const MD2Header *header;
	const MD2Command *commands;

};
