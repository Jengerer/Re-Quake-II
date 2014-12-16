#pragma once

#include "mesh.h"
#include "plane.h"
#include "quake2_common_define.h"
#include <allocatable.h>
#include <renderer/buffer_interface.h>
#include <renderer/index_buffer_interface.h>
#include <renderer/renderer_interface.h>
#include <renderer/resources_interface.h>
#include <inttypes.h>
#include <vector2.h>
#include <vector3.h>

namespace BSP
{

	// Vertex type for a face point.
	struct FaceVertex
	{
		Vector3 position;
		Vector2 texture;
		Vector2 lightMap;
	};
	typedef Mesh<FaceVertex> FaceMesh;

	// Class that wraps a map face.
	class Face : public Allocatable
	{

	public:

		Face();
		~Face();

		// Prepare face object.
		bool Initialize(int vertexCount);

		inline void SetVisibilityFrame(int32_t visibilityFrame) { this->visibilityFrame = visibilityFrame; }

		inline FaceMesh *GetMesh() { return &mesh; }
		inline bool IsVisible(int32_t currentVisibilityFrame) const { return (currentVisibilityFrame == visibilityFrame); }

		// Load renderer resources for this face.
		bool LoadResources(Renderer::Resources *resources);

		// Draw this face.
		void Draw(Renderer::Interface *renderer, Renderer::MaterialLayout *layout) const;

	private:

		FaceMesh mesh;

		// Renderer resources.
		Renderer::Buffer *vertexBuffer;
		
		// Visibility frame that this face is allowed to be drawn on.
		int32_t visibilityFrame;

	};

	// Class representing a non-leaf BSP node.
	class Node : public Allocatable
	{
	
	public:

		Node();
		~Node();

		void SetParameters(
			const Geometry::Plane *plane,
			int32_t frontChild,
			int32_t backChild,
			const Vector3 &minimums,
			const Vector3 &maximums,
			const BSP::Face *firstFace,
			uint16_t faceCount);

		inline void SetParent(BSP::Node *parent) { this->parent = parent; }
		inline void SetVisibilityFrame(int32_t visibilityFrame) { this->visibilityFrame = visibilityFrame; }

		inline const Geometry::Plane *GetPlane() const { return plane; }
		inline int32_t GetFrontChild() const { return frontChild; }
		inline int32_t GetBackChild() const { return backChild; }
		inline const Vector3 *GetMinimums() const { return &minimums; }
		inline const Vector3 *GetMaximums() const { return &maximums; }
		inline const BSP::Face *GetFirstFace() const { return firstFace; }
		inline uint16_t GetFaceCount() const { return faceCount; }
		inline BSP::Node *GetParent() const { return parent; }
		inline uint32_t GetVisibilityFrame() const { return visibilityFrame; }

		inline bool IsVisible(int32_t visibilityFrame) const { return (visibilityFrame == this->visibilityFrame); }

	private:

		// Parameters from the raw map.
		const Geometry::Plane *plane;
		int32_t frontChild;
		int32_t backChild;
		Vector3 minimums;
		Vector3 maximums;
		const Face *firstFace;
		uint16_t faceCount;

		// Additional parameters for visibliity traversal.
		BSP::Node *parent;
		int32_t visibilityFrame;

	};

	// Class representing a brush side.
	class BrushSide : public Allocatable
	{

	public:

		BrushSide();
		~BrushSide();

		void SetParameters(
			const Geometry::Plane *plane,
			int16_t textureIndex);

		inline const Geometry::Plane *GetPlane() const { return plane; }
		inline int16_t GetTextureIndex() const { return textureIndex; }

	private:

		const Geometry::Plane *plane;
		int16_t textureIndex;

	};

	// Class representing a brush inside a leaf.
	class Brush : public Allocatable
	{

	public:

		Brush();
		~Brush();

		void SetParameters(
			const BSP::BrushSide *firstSide,
			int32_t sideCount,
			int32_t contents);

		inline const BSP::BrushSide *GetFirstSide() const { return firstSide; }
		inline int32_t GetSideCount() const { return sideCount; }
		inline int32_t GetContents() const { return contents; }

	private:

		const BSP::BrushSide *firstSide;
		int32_t sideCount;
		int32_t contents;

	};

	// Class representing a bit vector correspoding to the map clusters.
	class ClusterBitVector : public Allocatable
	{

	public:

		ClusterBitVector();
		~ClusterBitVector();

		// Decompress the visibility set to a full bit vector.
		void Decompress(int32_t clusterCount, uint8_t *out) const;

		inline void SetStart(const uint8_t *start) { this->start = start; }

	private:

		const uint8_t *start;

	public:

		static const int32_t ClustersPerElement = 8; // Number of clusters in a single byte.
		static const uint32_t ElementIndexShift = 3; // Bits to shift to get decompressed element for a cluster.
		static const uint32_t BitIndexMask = 7; // Mask for retrieving bit index in an element.

	};

	// Visibility cluster containing both a visibility set for potentially visible
	// and potentially audible leaves.
	class LeafCluster : public Allocatable
	{

	public:

		LeafCluster();
		~LeafCluster();

		void SetParameters(
			const uint8_t *visibleData,
			const uint8_t *audibleData);

		inline const BSP::ClusterBitVector *GetVisibilitySet() const { return &visibleSet; }
		inline const BSP::ClusterBitVector *GetAudibilitySet() const { return &audibleSet; }
		
	private:

		BSP::ClusterBitVector visibleSet;
		BSP::ClusterBitVector audibleSet;

	};

	// Class representing a leaf node in a BSP.
	class Leaf : public Allocatable
	{

	public:

		Leaf();
		~Leaf();

		void SetParameters(
			int32_t contents,
			int16_t clusterIndex,
			int16_t areaIndex,
			const Vector3 &minimums,
			const Vector3 &maximums,
			BSP::Face **faceTableStart,
			uint16_t faceCount,
			const BSP::Brush *firstBrush,
			uint16_t brushCount);

		void SetFacesVisible(int32_t currentVisibilityFrame);

		inline void SetParent(BSP::Node *parent) { this->parent = parent; }

		inline int16_t GetClusterIndex() const { return clusterIndex; }
		inline BSP::Node *GetParent() const { return parent; }

	private:

		// Parameters from map data.
		int32_t contents;
		int16_t clusterIndex;
		int16_t areaIndex;
		Vector3 minimums;
		Vector3 maximums;
		BSP::Face **faceTableStart;
		uint16_t faceCount;
		const BSP::Brush *firstBrush;
		uint16_t brushCount;

		// Additional data for visibility traversal.
		BSP::Node *parent;

	};

	// Class that wraps a BSP map.
	class Quake2CommonLibrary Map
	{

	public:

		Map();
		~Map();

		// Free all memory.
		void Destroy();

		// Prepare map segments to be filled out.
		bool InitializePlanes(int32_t planeCount);
		bool InitializeFaces(int32_t faceCount);
		bool InitializeNodes(int32_t nodeCount);
		bool InitializeBrushSides(int32_t brushSideCount);
		bool InitializeBrushes(int32_t brushCount);
		bool InitializeClusters(int32_t clusterCount, int32_t dataSize);
		bool InitializeLeafFacesTable(int32_t leafFaceIndexCount);
		bool InitializeLeaves(int32_t leafCount);

		// Populate the tree's ancestry information.
		void BuildParentGraph();

		// Map buffer functions.
		inline Geometry::Plane *GetPlanes() { return planes; }
		inline BSP::Node *GetNodes() { return nodes; }
		inline BSP::Face *GetFaces() { return faces; }
		inline BSP::BrushSide *GetBrushSides() { return brushSides; }
		inline BSP::Brush *GetBrushes() { return brushes; }
		inline BSP::LeafCluster *GetClusters() { return clusters; }
		inline uint8_t *GetClusterData() { return clusterData; }
		inline BSP::Face **GetLeafFaceTable() { return leafFaceTable; }
		inline BSP::Leaf *GetLeaves() { return leaves; }

		// Load renderer resources for the map.
		bool LoadResources(Renderer::Resources *resources);
		
		// Draw the map.
		void Draw(const Vector3 &viewPoint, Renderer::Interface *renderer);

	private:

		// Helper for building the ancestry graph.
		void BuildParentGraph(int32_t nodeIndex, BSP::Node *parent);

		// Find the leaf that a point is in.
		const BSP::Leaf *GetLeafByPoint(const Vector3 &point); // TODO: we can probably use referencePoint instead of passing.

		// Mark all leaves in a certain cluster as visible for the frame.
		void MarkVisibleCluster(int32_t clusterIndex);
		void SetParentsVisible(BSP::Leaf *leaf);

		// BSP tree draw helpers.
		void DrawNode(int32_t nodeIndex) const;

	public:

		// Get material layout for models.
		static bool LoadStaticResources(Renderer::Resources *resources, Renderer::Material *mapMaterial);
		static void FreeStaticResources();
		
		// Convert negative index to leaf index.
		static inline int32_t GetLeafIndex(int32_t index) { return (-1) - index; }

	private:

		// Map component arrays/lengths.
		Geometry::Plane *planes;
		BSP::Node *nodes;
		int32_t nodeCount; // TODO: if we loop through nodes as tree, This may not be needed either.
		BSP::Face *faces;
		int32_t faceCount;
		BSP::BrushSide *brushSides;
		BSP::Brush *brushes;
		BSP::LeafCluster *clusters;
		uint8_t *clusterData; // Compressed cluster data for all clusters.
		uint8_t *decompressedCluster; // Buffer for decompressed cluster data.
		int32_t clusterCount;
		BSP::Face **leafFaceTable;
		BSP::Leaf *leaves;
		int32_t leafCount;

		// Variables to use for drawing and leaf traversing (to avoid having to pass as parameters repeatedly).
		Vector3 referencePoint;
		Renderer::Interface *renderer;

		// Visibility information.
		int32_t visibleCluster;
		int32_t visibilityFrame;

	private:

		// Node constants.
		static const int32_t HeadIndex = 0;
		static const int32_t SolidLeaf = 0;

		// Cluster constants.
		static const int32_t InvalidClusterIndex = -1;

		// Map-generic material layout.
		static Renderer::MaterialLayout *layout;

	};

}