#pragma once

#include "engine_listener.h"
#include "engine_utilities.h"
#include "game_manager_interface.h"
#include "game_manager_utilities.h"
#include "game_manager_listener.h"
#include "input_listener.h"

namespace GameManager
{

	// Implementing class for engine listener and game module manager.
	class Implementation
		: public Interface,
		public InputListener,
		public Utilities,
		public Engine::Listener
	{

		Listener *listener;

	public:

		Implementation();
		~Implementation();

		// Get listener to pass to engine.
		virtual Engine::Listener *GetEngineListener();

		// Get input listener to pass to engine.
		virtual InputListener *GetInputListener();

		// Pass client and server listeners.
		virtual void SetListeners(GameManager::Listener *client, GameManager::Listener *server);

		// Engine listener functions.
		virtual bool OnInitialize(Engine::Utilities *engineUtilities);
		virtual void OnShutdown();
		virtual bool OnTick(float timeDelta);

		// Input listener functions.
		virtual InputEventResult OnKeyPress(KeyCode key);
		virtual InputEventResult OnKeyRelease(KeyCode key);

		// Game manager utilities for modules.
		virtual float GetTime() const;
		virtual void ClearScene();
		virtual void PresentFrame();
		virtual Renderer::Shader *CreateShader(const char *filename, Renderer::ShaderType type);
		virtual void DestroyShader(Renderer::Shader *shader);
		virtual Renderer::Program *CreateProgram(const Renderer::Shader *vertexShader, const Renderer::Shader *fragmentShader);
		virtual void DestroyProgram(Renderer::Program *program);
		virtual void SetProgram(const Renderer::Program *program);
		virtual void UnsetProgram(const Renderer::Program *program);
		virtual Renderer::ShaderSchema *CreateShaderSchema(
			const Renderer::Program *program,
			const Renderer::Attribute *attributes,
			int attributeCount);
		virtual void DestroyShaderSchema(Renderer::ShaderSchema *schema);
		virtual Renderer::Uniform *GetUniform(const Renderer::Program *program, const char *name);
		virtual void DestroyUniform(Renderer::Uniform *uniform);
		virtual void SetUniform(const Renderer::Uniform *uniform, const Matrix4x4 *matrix);

		// Singleton retriever.
		static Implementation *GetInstance();

	private:

		// Singleton instance.
		static Implementation instance;

		// Interface for retrieving assets from engine.
		Engine::Utilities *engineUtilities;

		// Game manager modules.
		GameManager::Listener *clientListener;

	};

}