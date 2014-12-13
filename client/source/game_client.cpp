#include "md2_parser.h"
#include "game_client.h"
#include <bsp_parser.h>
#include <math_common.h>
#include <memory_manager.h>
#include <stdio.h>

// Rendering parameters.
const char *ModelVertexShader = "md2.vert";
const char *ModelFragmentShader = "md2.frag";
const char *MapVertexShader = "bsp.vert";
const char *MapFragmentShader = "bsp.frag";
const float NearDistanceZ = 1.0f;
const float FarDistanceZ = 1024.0f;
const float AspectRatio = 4.0f / 3.0f;
const float FieldOfView = 90.0f;

Client::Client()
	: utilities(nullptr),
	modelMaterial(nullptr),
	modelObject(nullptr),
	modelView(nullptr),
	modelProjection(nullptr),
	mapProjection(nullptr)
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
	// Generate object matrix.
	Matrix4x4 objectMatrix;
	Matrix4x4 rotate;
	Matrix4x4 obj;
	Vector3 translation(0.f, -50.f, -150.f);
	objectMatrix.Translation(&translation);
	rotate.RotationY(angle);
	obj.Product(&objectMatrix, &rotate);
	angle += 1.f;

	// Draw model.
	renderer->SetMaterial(modelMaterial);
	modelObject->Set(&obj);
	model.Draw(renderer);
	renderer->UnsetMaterial(modelMaterial);

	// Draw map.
	renderer->SetMaterial(mapMaterial);
	map.Draw(renderer);
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
	modelObject = modelMaterial->GetVariable("object");
	if (modelObject == nullptr) {
		return false;
	}
	modelProjection = modelMaterial->GetVariable("projection");
	if (modelProjection == nullptr) {
		return false;
	}
	mapProjection = mapMaterial->GetVariable("projection");
	if (mapProjection == nullptr) {
		return false;
	}

	// Generate projection matrix.
	Matrix4x4 projectionMatrix;
	projectionMatrix.PerspectiveProjection(AspectRatio, FieldOfView, NearDistanceZ, FarDistanceZ);
	
	// Activate the material for setting variables.
	Renderer::Interface *renderer = utilities->GetRenderer();
	renderer->SetMaterial(modelMaterial);
	modelProjection->Set(&projectionMatrix);
	renderer->UnsetMaterial(modelMaterial);
	renderer->SetMaterial(mapMaterial);
	mapProjection->Set(&projectionMatrix);
	renderer->UnsetMaterial(mapMaterial);

	// Load static model resources.
	Renderer::Resources *resources = utilities->GetRendererResources();
	if (!EntityModel::LoadStaticResources(resources, modelMaterial)) {
		return false;
	}
	if (!BSP::Map::LoadStaticResources(resources, mapMaterial)) {
		return false;
	}

	// Load model.
	MD2Parser file;
	if (!file.Load("tris.md2", &model)) {
		return false;
	}
	if (!model.LoadResources(resources)) {
		return false;
	}

	// Load map.
	BSP::FileFormat::Parser bspParser;
	if (!bspParser.Load("city1.bsp", &map)) {
		return false;
	}
	if (!map.LoadResources(resources)) {
		return false;
	}
	return true;
}

// Free base resources for client.
void Client::FreeResources(void)
{
	// Destroy variables.
	if (modelProjection != nullptr) {
		modelProjection->Destroy();
	}
	if (modelView != nullptr) {
		modelView->Destroy();
	}
	if (modelObject != nullptr) {
		modelObject->Destroy();
	}
	if (mapProjection != nullptr) {
		mapProjection->Destroy();
	}

	// Destroy materials.
	if (modelMaterial != nullptr) {
		modelMaterial->Destroy();
	}
	if (mapMaterial != nullptr) {
		mapMaterial->Destroy();
	}

	// Destroy model.
	model.Destroy();
	map.Destroy();

	// Destroy static materials.
	EntityModel::FreeStaticResources();
	BSP::Map::FreeStaticResources();
}

// Initialize the game's shaders for rendering.
bool Client::InitializeShaders(void)
{
	// Set up programs.
	Renderer::Resources *resources = utilities->GetRendererResources();
	modelMaterial = resources->CreateMaterial(ModelVertexShader, ModelFragmentShader);
	if (modelMaterial == nullptr) {
		return false;
	}
	mapMaterial = resources->CreateMaterial(MapVertexShader, MapFragmentShader);
	if (mapMaterial == nullptr) {
		return false;
	}
	return true;
}
