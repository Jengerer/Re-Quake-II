#pragma once

#include "engine_listener.h"
#include "engine_utilities.h"
#include "game_manager_utilities.h"
#include "game_manager_listener.h"

namespace GameManager
{

	// Implementing class for engine listener and game module manager.
	class Implementation : public Engine::Listener, public GameManager::Utilities
	{

		Listener *listener;

	public:

		Implementation();
		~Implementation();

		// Pass engine interface.
		virtual void SetEngineUtilities(Engine::Utilities *utilities);

		// Engine listener functions.
		virtual bool OnInitialize();
		virtual void OnShutdown();
		virtual bool OnTick(float timeDelta);

		// Game manager utilities for modules.
		virtual float GetTime() const;
		virtual void ClearScene();
		virtual void PresentFrame();
		virtual Renderer::Shader *CreateShader(const char *filename, Renderer::ShaderType type);
		virtual void DestroyShader(Renderer::Shader *shader);
		virtual Renderer::Program *CreateProgram(const Renderer::Shader *vertexShader, const Renderer::Shader *fragmentShader);
		virtual void DestroyProgram(Renderer::Program *program);
		virtual Renderer::Uniform *GetUniform(const Renderer::Program *program, const char *name);
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