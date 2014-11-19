#include "md2_file.h"
#include "game_client.h"
#include "math_common.h"
#include "memory_manager.h"
#include <stdio.h>

// Rendering parameters.
const int PerspectiveShaderAttributeCount = 2;
const char *VertexShaderFile = "engine.vert";
const char *PixelShaderFile = "engine.frag";
const float NearDistanceZ = 1.0f;
const float FarDistanceZ = 1024.0f;
const float AspectRatio = 4.0f / 3.0f;
const float FieldOfView = 90.0f;

Client::Client()
	: utilities(nullptr),
	modelMaterial(nullptr),
	object(nullptr),
	view(nullptr),
	projection(nullptr)
{
}

// Initialize client.
bool Client::OnInitialized(GameManager::Utilities *utilities)
{
	this->utilities = utilities;
	if (!LoadResources()) {
		return false;
	}
	return true;
}

// Destroy client.
void Client::OnShutdown()
{
	FreeResources();
}

// Begin client frame.
bool Client::OnTickBegin()
{
	return true;
}

// Run main client frame.
bool Client::OnTick()
{
	return true;
}

// Run end client frame.
bool Client::OnTickEnd()
{
	static float angle = 0.f;
	Renderer::Interface *renderer = utilities->GetRenderer();
	renderer->ClearScene();
	renderer->SetMaterial(modelMaterial);
	// Generate object matrix.
	Matrix4x4 objectMatrix;
	Matrix4x4 rotate;
	Matrix4x4 obj;
	Vector3 translation(0.f, -50.f, -150.f);
	objectMatrix.Translation(&translation);
	rotate.RotationY(angle);
	obj.Product(&objectMatrix, &rotate);
	angle += 1.f;
	object->Set(&obj);
	
	model.Draw(renderer);
	renderer->UnsetMaterial(modelMaterial);
	utilities->PresentFrame();
	return true;
}

// Return singleton instance.
Client *Client::GetInstance()
{
	return &instance;
}

// Load all base resources required by client.
bool Client::LoadResources(void)
{
	// Prepare shaders.
	if (!InitializeShaders()) {
		return false;
	}

	// Get the location to the transform and projection matrix.
	object = modelMaterial->GetVariable("object");
	if (object == nullptr) {
		return false;
	}
	projection = modelMaterial->GetVariable("projection");
	if (projection == nullptr) {
		return false;
	}
	
	// Activate the material for setting variables.
	Renderer::Interface *renderer = utilities->GetRenderer();
	renderer->SetMaterial(modelMaterial);

	// Generate projection matrix.
	Matrix4x4 projectionMatrix;
	projectionMatrix.PerspectiveProjection(AspectRatio, FieldOfView, NearDistanceZ, FarDistanceZ);
	projection->Set(&projectionMatrix);

	// Unset material.
	renderer->UnsetMaterial(modelMaterial);

	// Load static model resources.
	Renderer::Resources *resources = utilities->GetRendererResources();
	if (!EntityModel::LoadStaticResources(resources, modelMaterial)) {
		return false;
	}

	// Load model.
	MD2File file(&model);
	if (!file.Load("tris.md2")) {
		return false;
	}
	if (!model.LoadResources(resources)) {
		return false;
	}
	return true;
}

// Free base resources for client.
void Client::FreeResources(void)
{
	// Destroy variables.
	if (projection != nullptr) {
		projection->Destroy();
	}
	if (view != nullptr) {
		view->Destroy();
	}
	if (object != nullptr) {
		object->Destroy();
	}

	// Destroy materials.
	if (modelMaterial != nullptr) {
		modelMaterial->Destroy();
	}

	// Destroy static materials.
	EntityModel::FreeStaticResources();
}

// Initialize the game's shaders for rendering.
bool Client::InitializeShaders(void)
{
	// Set up program.
	modelMaterial = utilities->GetRendererResources()->CreateMaterial(VertexShaderFile, PixelShaderFile);
	if (modelMaterial == nullptr) {
		return false;
	}
	return true;
}
