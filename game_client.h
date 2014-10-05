#pragma once

#include "renderer_shared.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"
#include "game_module.h"

// Game module for client.
class Client : public GameManager::Module
{

public:

	Client();

	// Game manager listener functions.
	virtual bool OnInitialized();
	virtual void OnShutdown();
	virtual bool OnTickBegin();
	virtual bool OnTick();
	virtual bool OnTickEnd();

	// Singleton instance retrieval.
	static Client *GetInstance();

private:

	// Resource loading functions.
	bool LoadResources();
	void FreeResources();
	bool InitializeShaders();

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

	// Singleton instance.
	static Client instance;

};