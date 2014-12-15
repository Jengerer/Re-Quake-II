#include "bsp_map.h"
#include <error_stack.h>

namespace BSP
{

	// Quake II map material layout.
	const int FaceAttributeCount = 3;
	const Renderer::Attribute FaceAttributes[FaceAttributeCount] =
	{
		Renderer::Attribute("position", Renderer::PositionType, Renderer::Vector3Type),
		Renderer::Attribute("uv", Renderer::TextureCoordinateType, Renderer::Vector2Type),
		Renderer::Attribute("lightMapUV", Renderer::TextureCoordinateType, Renderer::Vector2Type)
	};
	const int QuakeMapBufferCount = 1;
	const Renderer::BufferLayout QuakeMapBufferLayouts[QuakeMapBufferCount] =
	{
		Renderer::BufferLayout(FaceAttributes, FaceAttributeCount)
	};
	const int FaceBufferIndex = 0;

	Face::Face() : vertexBuffer(nullptr)
	{
	}

	Face::~Face()
	{
		if (vertexBuffer != nullptr) {
			vertexBuffer->Destroy();
		}
	}

	bool Face::Initialize(int vertexCount)
	{
		if (!mesh.Initialize(vertexCount)) {
			ErrorStack::Log("Failed to allocate %d vertices for map face.", vertexCount);
			return false;
		}
		return true;
	}

	// Load face vertices into renderer buffer.
	bool Face::LoadResources(Renderer::Resources *resources)
	{
		const void *vertexData = mesh.GetVertexBuffer();
		unsigned int vertexBufferSize = mesh.GetVertexBufferSize();
		Renderer::Buffer *buffer = resources->CreateBuffer(vertexData, vertexBufferSize);
		if (buffer == nullptr) {
			return false;
		}
		this->vertexBuffer = buffer;
		return true;
	}

	// Draw this face.
	void Face::Draw(Renderer::Interface *renderer, Renderer::MaterialLayout *layout)
	{
		const int vertexCount = mesh.GetVertexCount();
		layout->BindBuffer(FaceBufferIndex, vertexBuffer);
		renderer->Draw(Renderer::TriangleFan, vertexCount);
	}

	Node::Node()
	{
	}

	Node::~Node()
	{
	}

	void Node::SetParameters(
		const Geometry::Plane *plane,
		int32_t frontChild,
		int32_t backChild,
		const Vector3 &minimums,
		const Vector3 &maximums,
		const BSP::Face *firstFace,
		uint16_t faceCount)
	{
		this->plane = plane;
		this->frontChild = frontChild;
		this->backChild = backChild;
		this->minimums = minimums;
		this->maximums = maximums;
		this->firstFace = firstFace;
		this->faceCount = faceCount;
	}

	BrushSide::BrushSide()
	{
	}

	BrushSide::~BrushSide()
	{
	}

	void BrushSide::SetParameters(
		const Geometry::Plane *plane,
		int16_t textureIndex)
	{
		this->plane = plane;
		this->textureIndex = textureIndex;
	}

	Brush::Brush()
	{
	}

	Brush::~Brush()
	{
	}

	void Brush::SetParameters(
		const BSP::BrushSide *firstSide,
		int32_t sideCount,
		int32_t contents)
	{
		this->firstSide = firstSide;
		this->sideCount = sideCount;
		this->contents = contents;
	}

	Leaf::Leaf()
	{
	}

	Leaf::~Leaf()
	{
	}

	void Leaf::SetParameters(
		int32_t contents,
		int16_t visibilityCluster,
		int16_t areaIndex,
		const Vector3 &minimums,
		const Vector3 &maximums,
		const BSP::Face *firstFace,
		uint16_t faceCount,
		const BSP::Brush *firstBrush,
		uint16_t brushCount)
	{
		this->contents = contents;
		this->visibilityCluster = visibilityCluster;
		this->areaIndex = areaIndex;
		this->minimums = minimums;
		this->maximums = maximums;
		this->firstFace = firstFace;
		this->faceCount = faceCount;
		this->firstBrush = firstBrush;
		this->brushCount = brushCount;
	}

	// Map-generic renderer resource definitions.
	Renderer::MaterialLayout *Map::layout = nullptr;

	Map::Map()
		: planes(nullptr),
		nodes(nullptr),
		faces(nullptr)
	{
	}

	Map::~Map()
	{
		Destroy();
	}

	// Delete map resources.
	void Map::Destroy()
	{
		delete[] planes;
		planes = nullptr;
		delete[] nodes;
		nodes = nullptr;
		delete[] faces;
		faces = nullptr;
	}

	bool Map::InitializePlanes(int32_t planeCount)
	{
		planes = new Geometry::Plane[planeCount];
		if (planes == nullptr) {
			ErrorStack::Log("Failed to allocate %d planes for map.", planeCount);
			return false;
		}
		return true;
	}

	bool Map::InitializeNodes(int32_t nodeCount)
	{
		nodes = new BSP::Node[nodeCount];
		if (planes == nullptr) {
			ErrorStack::Log("Failed to allocate %d nodes.", nodeCount);
			return false;
		}
		this->nodeCount = nodeCount;
		return true;
	}

	bool Map::InitializeFaces(int32_t faceCount)
	{
		faces = new BSP::Face[faceCount];
		if (faces == nullptr) {
			ErrorStack::Log("Failed to allocate %d faces for map.", faceCount);
			return false;
		}
		this->faceCount = faceCount;
		return true;
	}

	bool Map::InitializeBrushSides(int32_t brushSideCount)
	{
		brushSides = new BSP::BrushSide[brushSideCount];
		if (brushSides == nullptr) {
			ErrorStack::Log("Failed to allocate %d brush sides for map.", brushSideCount);
			return false;
		}
		return true;
	}

	bool Map::InitializeBrushes(int32_t brushCount)
	{
		brushes = new BSP::Brush[brushCount];
		if (brushes == nullptr) {
			ErrorStack::Log("Failed to allocate %d brushes for map.", brushes);
			return false;
		}
		return true;
	}

	bool Map::InitializeLeaves(int32_t leafCount)
	{
		leaves = new BSP::Leaf[leafCount];
		if (leaves == nullptr) {
			ErrorStack::Log("Failed to allocate %d leaves for map.", leafCount);
			return false;
		}
		return true;
	}

	// Load the map renderer resources.
	bool Map::LoadResources(Renderer::Resources *resources)
	{
		Face *currentFace = this->faces;
		int32_t faceCount = this->faceCount;
		for (int32_t i = 0; i < faceCount; ++i, ++currentFace) {
			if (!currentFace->LoadResources(resources)) {
				return false;
			}
		}
		return true;
	}

	// Draw the map.
	void Map::Draw(Renderer::Interface *renderer)
	{
		Face *currentFace = this->faces;
		int32_t faceCount = this->faceCount;
		for (int32_t i = 0; i < faceCount; ++i, ++currentFace) {
			currentFace->Draw(renderer, layout);
		}
	}

	// Get generic map renderer resources.
	bool Map::LoadStaticResources(Renderer::Resources *resources, Renderer::Material *mapMaterial)
	{
		layout = mapMaterial->GetLayout(QuakeMapBufferLayouts, QuakeMapBufferCount);
		if (layout == nullptr) {
			return false;
		}
		return true;
	}

	// Free generic map renderer resources.
	void Map::FreeStaticResources()
	{
		if (layout != nullptr) {
			layout->Destroy();
		}
	}

}
