#pragma once

#include "renderer_shared.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"
#include "game_module.h"

// Game module for client.
class Client : public GameManager::Module
{

public:

	// Game manager listener functions.
	virtual bool OnInitialized();
	virtual void OnShutdown();
	virtual bool OnTickBegin();
	virtual bool OnTick();
	virtual bool OnTickEnd();

private:

	// Shader parameters.
	Renderer::Shader *modelShader;
	Renderer::Shader *textureShader;
	Renderer::Program *modelProgram;
	Renderer::ShaderSchema *modelSchema;

	// Uniform variables for rendering.
	Renderer::Uniform *object;
	Renderer::Uniform *view;
	Renderer::Uniform *projection;

};