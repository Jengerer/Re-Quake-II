#include "md2_parser.h"
#include "game_client.h"
#include <bsp_parser.h>
#include <image.h>
#include <math_common.h>
#include <memory_manager.h>
#include <pcx_parser.h>
#include <quake_file_manager.h>
#include <stdio.h>
#include <wal_parser.h>

// Rendering parameters.
const char *ModelVertexShader = "md2.vert";
const char *ModelFragmentShader = "md2.frag";
const float NearDistanceZ = 4.0f;
const float FarDistanceZ = 4096.0f;
const float AspectRatio = 4.0f / 3.0f;
const float FieldOfView = 90.0f;

Client::Client()
	: utilities(nullptr),
	modelMaterial(nullptr),
	modelObject(nullptr),
	modelProjectionView(nullptr)
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
	Matrix4x4 projectionView;
	camera.GenerateProjectionView(&projectionView);

	// Multiply view by projection.

	// Generate object matrix.
	Matrix4x4 objectMatrix;
	Matrix4x4 rotate;
	Matrix4x4 obj;
	Vector3 translation(0.f, -50.f, -75.f);
	objectMatrix.Translation(&translation);
	rotate.RotationY(angle);
	obj.Product(&objectMatrix, &rotate);
	angle += 1.f;

	// Draw map.
	const Vector3 *cameraPosition = camera.GetPosition();
	map.Draw(renderer, *cameraPosition, projectionView);

	// Draw model.
	renderer->SetMaterial(modelMaterial);
	modelObject->SetMatrix4x4(&obj);
	modelProjectionView->SetMatrix4x4(&projectionView);
	renderer->SetTexture(modelSkin, 0);
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
	modelProjectionView = modelMaterial->GetVariable("projectionView");
	if (modelProjectionView == nullptr) {
		return false;
	}
	modelTexture = modelMaterial->GetVariable("texture");
	if (modelTexture == nullptr) {
		return false;
	}

	// Prepare PAK files.
	if (!QuakeFileManager::Initialize()) {
		return false;
	}
	
	// Prepare to load game resources.
	Renderer::Resources *resources = utilities->GetRendererResources();

	// Initialize map painter.
	if (!BSP::Painter::Initialize(resources)) {
		return false;
	}

	// Load the palette.
	if (!WAL::Parser::LoadPalette()) {
		return false;
	}

	// Load map.
	BSP::FileFormat::Parser bspParser;
	if (!bspParser.Load("maps/city1.bsp", &map)) {
		return false;
	}
	if (!map.LoadResources(resources)) {
		return false;
	}

	// Load model.
	MD2::Parser md2Parser;
	if (!md2Parser.Load("models/monsters/bitch/tris.md2", &model)) {
		return false;
	}
	if (!model.LoadResources(resources)) {
		return false;
	}

	// Load texture.
	Image<PixelRGBA> image;
	PCX::Parser pcxParser;
	if (!pcxParser.Load("models/monsters/bitch/skin.pcx", &image)) {
		return false;
	}

	// Create a texture.
	modelSkin = resources->CreateTexture(&image);
	if (modelSkin == nullptr) {
		return false;
	}

	// Set up camera projection.
	camera.SetFrustum(NearDistanceZ, FarDistanceZ, AspectRatio, FieldOfView);

	// Activate the material for setting variables.
	Renderer::Interface *renderer = utilities->GetRenderer();
	renderer->SetMaterial(modelMaterial);
	modelTexture->SetInteger(0);
	renderer->UnsetMaterial(modelMaterial);

	// Load static model resources.
	if (!EntityModel::LoadStaticResources(resources, modelMaterial)) {
		return false;
	}
	return true;
}

// Free base resources for client.
void Client::FreeResources(void)
{
    // Free variables.
    delete modelProjectionView;
    modelProjectionView = nullptr;
    delete modelObject;
    modelObject = nullptr;
    delete modelTexture;
    modelTexture = nullptr;

	// Destroy textures.
    delete modelSkin;
    modelSkin = nullptr;

	// Destroy materials.
    delete modelMaterial;
    modelMaterial = nullptr;

	// Destroy model.
	model.Destroy();
	map.Destroy();

	// Destroy static materials.
	EntityModel::FreeStaticResources();
	BSP::Painter::Shutdown();
	WAL::Parser::DestroyPalette();
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
	return true;
}
