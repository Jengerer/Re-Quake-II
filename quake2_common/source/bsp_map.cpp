#include "bsp_map.h"
#include <error_stack.h>
#include <stdio.h> // TODO: remove.

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
	void Face::Draw(Renderer::Interface *renderer, Renderer::MaterialLayout *layout) const
	{
		const int vertexCount = mesh.GetVertexCount();
		layout->BindBuffer(FaceBufferIndex, vertexBuffer);
		renderer->Draw(Renderer::TriangleFan, vertexCount);
	}

	Node::Node() : visibilityFrame(-1)
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

	ClusterBitVector::ClusterBitVector()
	{
	}

	ClusterBitVector::~ClusterBitVector()
	{
	}

	// Decompress the set to a bit vector.
	// Assumes output array has leafCount / 8 elements (rounded up).
	void ClusterBitVector::Decompress(int32_t clusterCount, uint8_t *out) const
	{
		int32_t elementsWritten = 0;
		const uint8_t *input = start;
		while (clusterCount > 0) {
			// If this character is non-zero, it's a straight copy.
			uint8_t current = *input++;
			if (current != 0) {
				*out++ = current;
				clusterCount -= ClustersPerElement;
				++elementsWritten;
			}
			else {
				// If it's zero, the next element is the amount of compressed zero elements following.
				uint8_t skipped = *input++;
				clusterCount -= (skipped * ClustersPerElement);
				elementsWritten += skipped;
				while (skipped-- != 0) {
					*out++ = 0;
				}
			}
		}
		elementsWritten++;
	}

	LeafCluster::LeafCluster()
	{
	}

	LeafCluster::~LeafCluster()
	{
	}

	void LeafCluster::SetParameters(
		const uint8_t *visibleData,
		const uint8_t *audibleData)
	{
		visibleSet.SetStart(visibleData);
		audibleSet.SetStart(audibleData);
	}

	// Some leaves are orphaned, so default to no parents in case not traversed.
	Leaf::Leaf() : parent(nullptr)
	{
	}

	Leaf::~Leaf()
	{
	}

	void Leaf::SetParameters(
		int32_t contents,
		int16_t clusterIndex,
		int16_t areaIndex,
		const Vector3 &minimums,
		const Vector3 &maximums,
		const BSP::Face *firstFace,
		uint16_t faceCount,
		const BSP::Brush *firstBrush,
		uint16_t brushCount)
	{
		this->contents = contents;
		this->clusterIndex = clusterIndex;
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

	// Start frame to get incremented to 0 on first frame.
	Map::Map()
		: planes(nullptr),
		nodes(nullptr),
		faces(nullptr),
		visibilityFrame(-1)
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
		delete[] brushSides;
		brushSides = nullptr;
		delete[] brushes;
		brushes = nullptr;
		delete[] clusters;
		clusters = nullptr;
		delete[] leaves;
		leaves = nullptr;

		// Data is allocated manually.
		if (clusterData != nullptr) {
			MemoryManager::Free(clusterData);
			clusterData = nullptr;
		}
		if (decompressedCluster != nullptr) {
			MemoryManager::Free(decompressedCluster);
			decompressedCluster = nullptr;
		}
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

	bool Map::InitializeClusters(int32_t clusterCount, int32_t dataSize)
	{
		clusters = new BSP::LeafCluster[clusterCount];
		if (clusters == nullptr) {
			ErrorStack::Log("Failed to allocate %d clusters for map.", clusterCount);
			return false;
		}
		clusterData = reinterpret_cast<uint8_t*>(MemoryManager::Allocate(dataSize));
		if (clusterData == nullptr) {
			ErrorStack::Log("Failed to allocate %d bytes for cluster data.", dataSize);
			return false;
		}

		// Allocate space for decompressed cluster.
		// Get ceiling of number of elements needed.
		uint32_t elementCount = (clusterCount + (ClusterBitVector::ClustersPerElement - 1)) / ClusterBitVector::ClustersPerElement;
		decompressedCluster = reinterpret_cast<uint8_t*>(MemoryManager::Allocate(elementCount));
		if (decompressedCluster == nullptr) {
			ErrorStack::Log("Failed to allocate cluster buffer of size %d for %d leaves.", elementCount, leafCount);
			return false;
		}
		this->clusterCount = clusterCount;

		// Start visible cluster to sentinel index past array end.
		visibleCluster = clusterCount;
		return true;
	}

	bool Map::InitializeLeaves(int32_t leafCount)
	{
		leaves = new BSP::Leaf[leafCount];
		if (leaves == nullptr) {
			ErrorStack::Log("Failed to allocate %d leaves for map.", leafCount);
			return false;
		}
		this->leafCount = leafCount;
		return true;
	}

	// Build the graph so each node and leaf references its parent.
	void Map::BuildParentGraph()
	{
		BuildParentGraph(HeadIndex, nullptr);
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
	void Map::Draw(const Vector3 &viewPoint, Renderer::Interface *renderer)
	{
		// Store parameters so we don't have to keep passing them down.
		this->referencePoint = viewPoint;
		this->renderer = renderer;

		// Find the leaf node that camera is in.
		const BSP::Leaf *viewLeaf = GetLeafByPoint(referencePoint);
		int16_t viewClusterIndex = viewLeaf->GetClusterIndex();

		// Check if we need to update visibility frame.
		if (viewClusterIndex != visibleCluster) {
			++visibilityFrame;
			visibleCluster = viewClusterIndex;
			MarkVisibleCluster(viewClusterIndex);
		}
		
		// Start drawing from head of tree.
		const BSP::Node *node = &nodes[HeadIndex];
		DrawNode(node);
	}

	// Build the parent graph from a given node.
	void Map::BuildParentGraph(int32_t nodeIndex, BSP::Node *parent)
	{
		if (nodeIndex < 0) {
			nodeIndex = GetLeafIndex(nodeIndex);
			BSP::Leaf *leaf = &leaves[nodeIndex];
			leaf->SetParent(parent);
		}
		else {
			BSP::Node *node = &nodes[nodeIndex];
			node->SetParent(parent);

			// Recurse into children.
			BuildParentGraph(node->GetFrontChild(), node);
			BuildParentGraph(node->GetBackChild(), node);
		}
	}

	// Get the leaf that a certain point is in.
	const BSP::Leaf *Map::GetLeafByPoint(const Vector3 &point)
	{
		const BSP::Node *node = &nodes[HeadIndex];
		while (node != nullptr) {
			const Geometry::Plane *plane = node->GetPlane();
			int32_t nextIndex;
			if (plane->IsPointInFront(point)) {
				nextIndex = node->GetFrontChild();
			}
			else {
				nextIndex = node->GetBackChild();
			}

			// Check if we should traverse or stop.
			if (nextIndex < 0) {
				nextIndex = GetLeafIndex(nextIndex);
				return &leaves[nextIndex];
			}
			else {
				node = &nodes[nextIndex];
			}
		}

		// Unreachable: should always hit a leaf.
		return nullptr;
	}

	// Mark all leaves in a given cluster for drawing.
	void Map::MarkVisibleCluster(int32_t clusterIndex)
	{
		int32_t leafCount = this->leafCount;
		BSP::Leaf *leaf = leaves;

		// If no cluster, mark all as visible.
		if (clusterIndex == InvalidClusterIndex) {
			for (int32_t i = 0; i < leafCount; ++i, ++leaf) {
				SetParentsVisible(leaf);
			}
		}
		else {
			// Decompress the visibility set.
			const BSP::LeafCluster *cluster = &clusters[clusterIndex];
			const BSP::ClusterBitVector *visibilitySet = cluster->GetVisibilitySet();
			visibilitySet->Decompress(clusterCount, decompressedCluster);
			for (int32_t i = 0; i < leafCount; ++i, ++leaf) {
				// Check if the leaf's cluster is visible.
				int32_t leafClusterIndex = leaf->GetClusterIndex();
				if (leafClusterIndex == InvalidClusterIndex) {
					continue;
				}
				int32_t elementIndex = (leafClusterIndex >> BSP::ClusterBitVector::ElementIndexShift);
				uint8_t bitIndex = (clusterIndex & BSP::ClusterBitVector::BitIndexMask);
				if ((decompressedCluster[elementIndex] & (1 << bitIndex)) != 0) {
					SetParentsVisible(leaf);
				}
			}
		}
	}

	// Mark the ancestors of a given leaf as visible.
	void Map::SetParentsVisible(BSP::Leaf *leaf)
	{
		BSP::Node *parent = leaf->GetParent();
		while (parent != nullptr) {
			// Check if another leaf has already traversed this ancestor.
			uint32_t currentFrame = parent->GetVisibilityFrame();
			if (currentFrame == visibilityFrame) {
				break;
			}
			parent->SetVisibilityFrame(visibilityFrame);
			parent = parent->GetParent();
		}
	}

	// Draw the map from the given node.
	void Map::DrawNode(const BSP::Node *node) const
	{
		// Check if this node is visible.
		if (!node->IsVisible(visibilityFrame)) {
			return;
		}

		// Check which child to draw first.
		int32_t nearChild;
		int32_t farChild;
		const Geometry::Plane *plane = node->GetPlane();
		if (plane->IsPointInFront(referencePoint)) {
			nearChild = node->GetFrontChild();
			farChild = node->GetBackChild();
		}
		else {
			nearChild = node->GetBackChild();
			farChild = node->GetFrontChild();
		}

		// Recurse into near child.
		if (nearChild >= 0) {
			DrawNode(&nodes[nearChild]);
		}

		// Draw this node's faces.
		const BSP::Face *face = node->GetFirstFace();
		int32_t faceCount = node->GetFaceCount();
		for (int32_t i = 0; i < faceCount; ++i, ++face) {
			face->Draw(renderer, layout);
		}

		// Recurse into far child.
		if (farChild >= 0) {
			DrawNode(&nodes[farChild]);
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
