#include "md2_parser.h"
#include "game_client.h"
#include <bsp_parser.h>
#include <math_common.h>
#include <memory_manager.h>
#include <pack_manager.h>
#include <stdio.h>

// Rendering parameters.
const char *ModelVertexShader = "md2.vert";
const char *ModelFragmentShader = "md2.frag";
const char *MapVertexShader = "bsp.vert";
const char *MapFragmentShader = "bsp.frag";
const float NearDistanceZ = 4.0f;
const float FarDistanceZ = 4096.0f;
const float AspectRatio = 4.0f / 3.0f;
const float FieldOfView = 90.0f;

Client::Client()
	: utilities(nullptr),
	modelMaterial(nullptr),
	modelObject(nullptr),
	modelView(nullptr),
	modelProjection(nullptr),
	mapView(nullptr),
	mapProjection(nullptr),
	mapColour(nullptr)
{
	camera.SetPosition(Vector3::Zero);
}

// Initialize client.
bool Client::OnInitialized(GameManager::Utilities *utilities)
{
	this->utilities = utilities;
	if (!LoadResources()) {
		return false;
	}

	// Reset mouse for the first time.
	int centerX, centerY;
	utilities->GetWindowSize(&centerX, &centerY);
	centerX >>= 1;
	centerY >>= 1;
	utilities->SetMousePosition(centerX, centerY);
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
	// Get mouse movement.
	int mouseX, mouseY;
	int centerX, centerY;
	utilities->GetMousePosition(&mouseX, &mouseY);
	utilities->GetWindowSize(&centerX, &centerY);
	centerX >>= 1;
	centerY >>= 1;
	float diffX = static_cast<float>(centerX - mouseX);
	float diffY = static_cast<float>(centerY - mouseY);

	// Turn camera.
	// Mouse X movement is rotation around Y.
	Vector3 turnAngles;
	turnAngles.Set(diffY, diffX, 0.f);
	camera.Turn(turnAngles);

	// Move camera forward.
	Vector3 forward;
	const Vector3 *cameraPosition = camera.GetPosition();
	camera.GetDirections(&forward, nullptr, nullptr);
	forward.ScalarMultiple(forward, 2.0f);
	forward.Sum(*cameraPosition, forward);
	camera.SetPosition(forward);

	// Reset cursor.
	utilities->SetMousePosition(centerX, centerY);
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

	// Generate view matrix.
	Matrix4x4 view;
	camera.GenerateViewTransform(&view);

	// Generate object matrix.
	Matrix4x4 objectMatrix;
	Matrix4x4 rotate;
	Matrix4x4 obj;
	Vector3 translation(0.f, -50.f, -150.f);
	objectMatrix.Translation(&translation);
	rotate.RotationY(angle);
	obj.Product(&objectMatrix, &rotate);
	angle += 1.f;

	// Draw map.
	const Vector3 *cameraPosition = camera.GetPosition();
	renderer->SetMaterial(mapMaterial);
	mapView->Set(&view);
	renderer->SetWireframe(false);
	Vector4 colour(1.f, 1.f, 1.f, 0.05f);
	mapColour->Set(&colour);
	map.Draw(*cameraPosition, renderer);
	renderer->UnsetMaterial(modelMaterial);

	// Draw model.
	renderer->SetMaterial(modelMaterial);
	modelObject->Set(&obj);
	view.Identity();
	modelView->Set(&view);
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
	modelObject = modelMaterial->GetVariable("object");
	if (modelObject == nullptr) {
		return false;
	}
	modelView = modelMaterial->GetVariable("view");
	if (modelView == nullptr) {
		return false;
	}
	modelProjection = modelMaterial->GetVariable("projection");
	if (modelProjection == nullptr) {
		return false;
	}
	mapView = mapMaterial->GetVariable("view");
	if (mapView == nullptr) {
		return false;
	}
	mapProjection = mapMaterial->GetVariable("projection");
	if (mapProjection == nullptr) {
		return false;
	}
	mapColour = mapMaterial->GetVariable("colour");
	if (mapColour == nullptr) {
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

	// Load packs.
	Pack::Manager manager;
	if (!manager.AddPack("pak0.pak")) {
		return false;
	}

	// Load map.
	FileData modelData;
	if (!manager.Read("maps/city1.bsp", &modelData)) {
		return false;
	}
	BSP::FileFormat::Parser bspParser;
	if (!bspParser.Load(modelData.GetData(), &map)) {
		return false;
	}
	if (!map.LoadResources(resources)) {
		return false;
	}

	// Load model.
	MD2Parser md2Parser;
	if (!manager.Read("models/monsters/bitch/tris.md2", &modelData)) {
		return false;
	}
	if (!md2Parser.Load(modelData.GetData(), &model)) {
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
	if (modelProjection != nullptr) {
		modelProjection->Destroy();
	}
	if (modelView != nullptr) {
		modelView->Destroy();
	}
	if (modelObject != nullptr) {
		modelObject->Destroy();
	}
	if (mapView != nullptr) {
		mapView->Destroy();
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
	if (mapColour != nullptr) {
		mapColour->Destroy();
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
