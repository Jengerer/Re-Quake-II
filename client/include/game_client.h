#pragma once

#include "renderer/material_interface.h"
#include "renderer/shared.h"
#include "renderer/variable_interface.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"
#include "game_module.h"

// Game module for client.
class Client : public GameModule
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
	Renderer::Material *model;

	// Uniform variables for rendering.
	Renderer::Variable *object;
	Renderer::Variable *view;
	Renderer::Variable *projection;

	// Singleton instance.
	static Client instance;

};