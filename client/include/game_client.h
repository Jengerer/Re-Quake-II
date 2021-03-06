#pragma once

#include "camera.h"
#include "entity_model.h"
#include <bsp_map.h>
#include <renderer/material_interface.h>
#include <renderer/shared.h>
#include <renderer/variable_interface.h>
#include <game_manager_listener.h>
#include <game_manager_utilities.h>
#include <game_module.h>

// Game module for client.
class Client : public GameModule
{

public:

	Client();

	// Game manager listener functions.
	virtual bool OnInitialized(GameManager::Utilities *utilities);
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

	// Reference for game manager utilities.
	GameManager::Utilities *utilities;

	// Shader parameters.
	Renderer::Texture *modelSkin;
	Renderer::Material *modelMaterial;

	// Uniform variables for rendering.
	Renderer::Variable *modelObject;
	Renderer::Variable *modelProjectionView;
	Renderer::Variable *modelTexture;

	// Model and map to render.
	Camera camera;
	EntityModel model;
	BSP::Map map;

private:

	// Singleton instance.
	static Client instance;

};