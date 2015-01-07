#pragma once

#include "mesh.h"
#include "quake2_common_define.h"
#include <allocatable.h>
#include <renderer/renderer_interface.h>
#include <renderer/resources_interface.h>

namespace BSP
{

	// Vertex type for a face point.
	struct FaceVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector2 lightMap;
	};
	typedef Mesh<FaceVertex> FaceMesh;

	// Singleton that handles rendering for a BSP map.
	class Painter : public Allocatable
	{

	public:

		// Initialize rendering parameters.
		static bool Quake2CommonLibrary Initialize(Renderer::Resources *resources);
		static void Quake2CommonLibrary Shutdown();

	public:

		// Set up the renderer for drawing from a given perspective.
		void PrepareRenderer(
			Renderer::Interface *renderer,
			const Matrix4x4 &projectionView); 

		// Clear the renderer from drawing the map.
		void ClearRenderer(Renderer::Interface *renderer);

		// Set the face texture to use for drawing.
		void SetTexture(
			Renderer::Interface *renderer,
			Renderer::Texture *texture,
			const Vector2 &textureSize);

		// Make a draw call for a certain set of face triangles.
		void DrawFace(
			Renderer::Interface *renderer,
			Renderer::Buffer *buffer,
			unsigned int vertexCount);

	private:

		Painter();
		~Painter();

		// Initialize the renderer paramaters for drawing.
		bool LoadResources(Renderer::Resources *resources);

	public:

		static Painter *instance;

	private:

		Renderer::Material *material;
		Renderer::MaterialLayout *layout;
		Renderer::Variable *projectionViewVariable;
		Renderer::Variable *textureSlotVariable;
		Renderer::Variable *textureSizeVariable;

	};

}