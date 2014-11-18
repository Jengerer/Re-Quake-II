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

// Private functions.
static void client_load_resources(void);
static void client_free_resources(void);
static void client_initialize_shaders(void);

Client::Client()
	: model(nullptr),
	object(nullptr),
	view(nullptr),
	projection(nullptr)
{
}

// Initialize client.
bool Client::OnInitialized()
{
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
	GameManager::Utilities::renderer->ClearScene();
	GameManager::Utilities::instance->PresentFrame();
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
	if (!InitializeShaders()) {
		return false;
	}

	// Get the location to the transform and projection matrix.
	object = model->GetVariable("object");
	if (object == nullptr) {
		return false;
	}
	projection = model->GetVariable("projection");
	if (projection == nullptr) {
		return false;
	}
	
	// Generate projection matrix.
	Matrix4x4 projectionMatrix;
	projectionMatrix.PerspectiveProjection(AspectRatio, FieldOfView, NearDistanceZ, FarDistanceZ);
	projection->Set(&projectionMatrix);

	// Load model.
	EntityModel model;
	MD2File file(&model);
	if (!file.Load("tris.md2")) {
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
	if (model != nullptr) {
		model->Destroy();
	}
}

// Initialize the game's shaders for rendering.
bool Client::InitializeShaders(void)
{
	// Set up program.
	model = GameManager::Utilities::resources->CreateMaterial(VertexShaderFile, PixelShaderFile);
	if (model == nullptr) {
		return false;
	}
	return true;
}
