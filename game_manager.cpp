#include "game_manager.h"
#include "engine_interface.h"

// TODO: Erase these once we get settings.
const char *GameTitle = "Game Test";
const int GameWidth = 1024;
const int GameHeight = 768;

namespace GameManager
{

	// Clearing game manager constructor.
	Implementation::Implementation()
		: engineUtilities(nullptr)
	{
	}

	// Destroy game manager.
	Implementation::~Implementation()
	{
	}

	// Set interface for requesting engine resources.
	void Implementation::SetEngineUtilities(Engine::Utilities *utilities)
	{
		engineUtilities = utilities;
	}

	// Engine is set up.
	bool Implementation::OnInitialize()
	{
		if (!clientListener->OnInitialize()) {
			return false;
		}
		return true;
	}

	// Engine is shutting down, clean up.
	void Implementation::OnShutdown()
	{
		clientListener->OnShutdown();
	}

	// Engine frame update.
	bool Implementation::OnTick(float timeDelta)
	{
		clientListener->OnTickBegin();
		clientListener->OnTick();
		clientListener->OnTickEnd();
	}

	// Get current game time.
	float Implementation::GetTime() const
	{
		return 0.0f;
	}

	// Clear renderer for next frame.
	void Implementation::ClearScene()
	{
		Renderer::Interface *renderer = engineUtilities->GetRenderer();
		renderer->ClearScene();
	}

	// Swap buffers and show current render.
	void Implementation::PresentFrame()
	{
		engineUtilities->SwapBuffers();
	}

	// Create shader from source.
	Renderer::Shader *Implementation::CreateShader(const char *filename, Renderer::ShaderType type)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		return resources->CreateShader(filename, type);
	}

	// Destroy shader reference.
	void Implementation::DestroyShader(Renderer::Shader *shader)
	{
		if (shader != nullptr) {
			Renderer::Resources *resources = engineUtilities->GetRendererResources();
			resources->DestroyShader(shader);
		}
	}

	// Create program from vertex and fragment shader.
	Renderer::Program *Implementation::CreateProgram(const Renderer::Shader *vertexShader, const Renderer::Shader *fragmentShader)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		return resources->CreateProgram(vertexShader, fragmentShader);
	}

	// Clean up program.
	void Implementation::DestroyProgram(Renderer::Program *program)
	{
		if (program != nullptr) {
			Renderer::Resources *resources = engineUtilities->GetRendererResources();
			resources->DestroyProgram(program);
		}
	}

	// Get reference to shader uniform variable.
	// Assumes program is bound.
	Renderer::Uniform *Implementation::GetUniform(const Renderer::Program *program, const char *name)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		return resources->GetUniform(program, name);
	}

	// Set value of uniform by 4x4 matrix.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, const Matrix4x4 *matrix)
	{
		Renderer::Interface *renderer = engineUtilities->GetRenderer();
		renderer->SetUniform(uniform, matrix);
	}

	// Get singleton instance.
	Implementation *Implementation::GetInstance()
	{
		return &instance;
	}

}
