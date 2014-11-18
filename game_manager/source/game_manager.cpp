#include "game_manager.h"
#include "engine_interface.h"

// TODO: Erase these once we get settings.
const char *GameTitle = "Game Test";
const int GameWidth = 1024;
const int GameHeight = 768;

namespace GameManager
{

	// Singleton instance.
	Implementation Implementation::instance;

	// Clearing game manager constructor.
	Implementation::Implementation()
		: engineUtilities(nullptr)
	{
	}

	// Destroy game manager.
	Implementation::~Implementation()
	{
	}

	// Get interface for engine to pass events to.
	Engine::Listener *Implementation::GetEngineListener()
	{
		return static_cast<Engine::Listener*>(this);
	}

	// Get interface for engine to pass input events to.
	InputListener *Implementation::GetInputListener()
	{
		return static_cast<InputListener*>(this);
	}

	// Set client and server module listeners.
	void Implementation::SetListeners(GameManager::Listener *client, GameManager::Listener *server)
	{
		(void)server;
		this->clientListener = client;
	}

	// Engine is set up.
	bool Implementation::OnInitialize(Engine::Utilities *engineUtilities)
	{
		// Save the utilities interface.
		this->engineUtilities = engineUtilities;

		// Set up renderer and create window.
		WindowFlags flags;
		flags.raw = 0;
		if (!engineUtilities->MakeWindow(GameTitle, GameWidth, GameHeight, flags)) {
			return false;
		}

		// Pass to client to set up.
		if (!clientListener->OnInitialized(this)) {
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
		(float)timeDelta;
		if (!clientListener->OnTickBegin()) {
			return false;
		}
		if (!clientListener->OnTick()) {
			return false;
		}
		if (!clientListener->OnTickEnd()) {
			return false;
		}
		return true;
	}

	// Handle key press event.
	InputEventResult Implementation::OnKeyPress(KeyCode key)
	{
		(void)key;
		return InputEventSuccess;
	}

	// Handle key release event.
	InputEventResult Implementation::OnKeyRelease(KeyCode key)
	{
		(void)key;
		return InputEventSuccess;
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

	// Set program as active for rendering.
	void Implementation::SetProgram(const Renderer::Program *program)
	{
		Renderer::Interface *renderer = engineUtilities->GetRenderer();
		renderer->SetProgram(program);
	}

	// Unset program as active for rendering.
	void Implementation::UnsetProgram(const Renderer::Program *program)
	{
		Renderer::Interface *renderer = engineUtilities->GetRenderer();
		renderer->UnsetProgram(program);
	}

	// Create shader schema.
	Renderer::ShaderSchema *Implementation::CreateShaderSchema(
		const Renderer::Program *program,
		const Renderer::Attribute *attributes,
		int attributeCount)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		return resources->CreateShaderSchema(program, attributes, attributeCount);
	}

	// Destroy shader schema.
	void Implementation::DestroyShaderSchema(Renderer::ShaderSchema *schema)
	{
		if (schema != nullptr) {
			Renderer::Resources *resources = engineUtilities->GetRendererResources();
			resources->DestroyShaderSchema(schema);
		}
	}

	// Get reference to shader uniform variable.
	// Assumes program is bound.
	Renderer::Uniform *Implementation::GetUniform(const Renderer::Program *program, const char *name)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		return resources->GetUniform(program, name);
	}

	// Destroy uniform variable.
	void Implementation::DestroyUniform(Renderer::Uniform *uniform)
	{
		Renderer::Resources *resources = engineUtilities->GetRendererResources();
		resources->DestroyUniform(uniform);
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
