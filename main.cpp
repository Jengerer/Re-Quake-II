#include "engine_export.h"
#include "error_stack.h"
#include "game_manager_export.h"
#include "opengl_export.h"
#include "client_export.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// Get engine implementation.
	Engine::Interface *engine = GetEngineInterface();

	// Initialize error stack for error logging..
	ErrorStack::Initialize();

	// Initialize engine and run.
	bool success = engine->Initialize() && engine->Run();

	// How did we do?
	if (!success) {
		// Dump what went wrong.
		ErrorStack::Dump();

		// Pause at the end so we can read errors.
#if defined(_DEBUG)
		system("pause");
#endif
	}

	engine->Shutdown();
	return success;
}

