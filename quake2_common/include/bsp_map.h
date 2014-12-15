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

		// Get a reference to the mesh to be filled out.
		inline FaceMesh *GetMesh() { return &mesh; }

		// Load renderer resources for this face.
		bool LoadResources(Renderer::Resources *resources);

		// Draw this face.
		void Draw(Renderer::Interface *renderer, Renderer::MaterialLayout *layout);

	private:

		FaceMesh mesh;

		// Renderer resources.
		Renderer::Buffer *vertexBuffer;

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

		inline const Geometry::Plane *GetPlane() const { return plane; }
		inline int32_t GetFrontChild() const { return frontChild; }
		inline int32_t GetBackChild() const { return backChild; }
		inline const Vector3 *GetMinimums() const { return &minimums; }
		inline const Vector3 *GetMaximums() const { return &maximums; }
		inline const BSP::Face *GetFirstFace() const { return firstFace; }
		inline uint16_t GetFaceCount() const { return faceCount; }

	private:

		const Geometry::Plane *plane;
		int32_t frontChild;
		int32_t backChild;
		Vector3 minimums;
		Vector3 maximums;
		const Face *firstFace;
		uint16_t faceCount;

	};

	// Class that wraps a BSP map.
	class Quake2CommonLibrary Map
	{

	public:

		Map();
		~Map();

		// Prepare map segments to be filled out.
		bool InitializePlanes(int32_t planeCount);
		bool InitializeNodes(int32_t nodeCount);
		bool InitializeFaces(int32_t faceCount);

		// Free all memory.
		void Destroy();

		// Map buffer functions.
		inline Geometry::Plane *GetPlanes() { return planes; }
		inline int32_t GetPlaneCount() const { return planeCount; }
		inline Node *GetNodes() { return nodes; }
		inline int32_t GetNodeCount() const { return nodeCount; }
		inline Face *GetFaces() { return faces; }
		inline int32_t GetFaceCount() const { return faceCount;  }

		// Load renderer resources for the map.
		bool LoadResources(Renderer::Resources *resources);
		
		// Draw the map.
		void Draw(Renderer::Interface *renderer);

	public:

		// Get material layout for models.
		static bool LoadStaticResources(Renderer::Resources *resources, Renderer::Material *mapMaterial);
		static void FreeStaticResources();

	private:

		// Map component arrays/lengths.
		Geometry::Plane *planes;
		int32_t planeCount;
		Node *nodes;
		int32_t nodeCount;
		Face *faces;
		int32_t faceCount;

	private:

		// Map-generic material layout.
		static Renderer::MaterialLayout *layout;

	};

}