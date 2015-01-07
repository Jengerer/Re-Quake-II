#include "bsp_painter.h"
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
	const int FaceTextureSlot = 0;

	// Material parameters.
	const char *MapVertexShader = "bsp.vert";
	const char *MapFragmentShader = "bsp.frag";

	// Material variable names.
	const char *MapProjectionViewVariable = "projectionView";
	const char *MapTextureSlotVariable = "texture";
	const char *MapTextureSizeVariable = "textureSize";

	// Singleton instance reference.
	Painter *Painter::instance = nullptr;

	// Initialize painter singleton instance.
	// Returns true and fills out singleton pointer on success.
	bool Painter::Initialize(Renderer::Resources *resources)
	{
		instance = new Painter();
		if (instance == nullptr) {
			ErrorStack::Log("Failed to allocate BSP painter instance.");
			return false;
		}
		if (!instance->LoadResources(resources)) {
			Shutdown();
			return false;
		}
		return true;
	}

	// Shut down painter instance.
	void Painter::Shutdown()
	{
		delete instance;
		instance = nullptr;
	}

	// Prepare rendering for drawing faces from a given perspective.
	void Painter::PrepareRenderer(
		Renderer::Interface *renderer,
		const Matrix4x4 &projectionView)
	{
		renderer->SetMaterial(material);
		projectionViewVariable->SetMatrix4x4(&projectionView);
		textureSlotVariable->SetInteger(FaceTextureSlot);
	}

	// Clear the renderer from drawing faces.
	void Painter::ClearRenderer(Renderer::Interface *renderer)
	{
		renderer->UnsetMaterial(material);
	}

	// Set the texture to render the faces with.
	void Painter::SetTexture(
		Renderer::Interface *renderer,
		Renderer::Texture *texture,
		const Vector2 &textureSize)
	{
		renderer->SetTexture(texture, FaceTextureSlot);
		textureSizeVariable->SetVector2(&textureSize);
	}

	// Draw a face from a vertex buffer.
	void Painter::DrawFace(
		Renderer::Interface *renderer,
		Renderer::Buffer *buffer,
		unsigned int vertexCount)
	{
		layout->BindBuffer(FaceBufferIndex, buffer);
		renderer->SetMaterialLayout(layout);
		renderer->Draw(Renderer::TriangleFan, vertexCount);
		renderer->UnsetMaterialLayout(layout);
	}

	Painter::Painter()
		: material(nullptr),
		layout(nullptr),
		projectionViewVariable(nullptr),
		textureSlotVariable(nullptr),
		textureSizeVariable(nullptr)
	{
	}

	Painter::~Painter()
	{
        delete material;
        delete layout;
        delete projectionViewVariable;
        delete textureSlotVariable;
        delete textureSizeVariable;
	}

	// Load painter resources.
	bool Painter::LoadResources(Renderer::Resources *resources)
	{
		material = resources->CreateMaterial(MapVertexShader, MapFragmentShader);
		if (material == nullptr) {
			return false;
		}
		layout = material->GetLayout(QuakeMapBufferLayouts, QuakeMapBufferCount);
		if (layout == nullptr) {
			return false;
		}
		projectionViewVariable = material->GetVariable(MapProjectionViewVariable);
		if (projectionViewVariable == nullptr) {
			return false;
		}
		textureSlotVariable = material->GetVariable(MapTextureSlotVariable);
		if (textureSlotVariable == nullptr) {
			return false;
		}
		textureSizeVariable = material->GetVariable(MapTextureSizeVariable);
		if (textureSizeVariable == nullptr) {
			return false;
		}
		return true;
	}

}
