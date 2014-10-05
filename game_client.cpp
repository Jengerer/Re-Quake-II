#include "game_client.h"
#include "math_common.h"
#include "memory_manager.h"
#include <stdio.h>

// Rendering parameters.
const int PerspectiveShaderAttributeCount = 2;
const char *VertexShaderFile = "engine.vert";
const char *FragmentShaderFile = "engine.frag";
const float NearDistanceZ = 1.0f;
const float FarDistanceZ = 1024.0f;
const float AspectRatio = 4.0f / 3.0f;
const float FieldOfView = 90.0f;

// Renderer shader schemas.
const Renderer::Attribute ModelAttributes[PerspectiveShaderAttributeCount] = {
	Renderer::Attribute(VertexShaderFile, Renderer::Vector3Type),
	Renderer::Attribute(FragmentShaderFile, Renderer::Vector2Type)
};

// Private functions.
static void client_load_resources(void);
static void client_free_resources(void);
static void client_initialize_shaders(void);

Client::Client()
	: modelShader(nullptr),
	textureShader(nullptr),
	modelProgram(nullptr),
	modelSchema(nullptr),
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
	InitializeShaders();

	// Set up the program for retrieving the uniforms.
	utilities->SetProgram(modelProgram);

	// Get the location to the transform and projection matrix.
	object = utilities->GetUniform(modelProgram, "object");
	projection = utilities->GetUniform(modelProgram, "projection");
	
	// Generate projection matrix.
	Matrix4x4 projectionMatrix;
	projectionMatrix.PerspectiveProjection(AspectRatio, FieldOfView, NearDistanceZ, FarDistanceZ);
	utilities->SetUniform(projection, &projectionMatrix);

	// Unset program.
	utilities->UnsetProgram();
}

// Free base resources for client.
void Client::FreeResources(void)
{
	// Free uniforms.
	utilities->DestroyUniform(object);
	utilities->DestroyUniform(view);
	utilities->DestroyUniform(projection);

	// Destroy schema.
	utilities->DestroyShaderSchema(modelSchema);

	// Destroy program and shaders.
	utilities->DestroyProgram(modelProgram);
	utilities->DestroyShader(modelShader);
	utilities->DestroyShader(textureShader);
}

// Initialize the game's shaders for rendering.
bool Client::InitializeShaders(void)
{
	// Set up program.
	modelShader = utilities->CreateShader(VertexShaderFile, Renderer::VertexShader);
	if (modelShader == nullptr) {
		return false;
	}
	textureShader = utilities->CreateShader(FragmentShaderFile, Renderer::FragmentShader);
	if (textureShader == nullptr) {
		return false;
	}
	modelProgram = utilities->CreateProgram(modelShader, textureShader);
	if (modelProgram == nullptr) {
		return false;
	}

	// Get the renderer schema for the program.
	modelSchema = utilities->CreateShaderSchema(modelProgram, ModelAttributes, PerspectiveShaderAttributeCount);
	if (modelSchema == nullptr) {
		return false;
	}
	return true;
}
