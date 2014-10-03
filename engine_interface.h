#pragma once

// Interface for an application to interact with the engine.
class EngineInterface
{
public:

	// Engine initialization.
	virtual bool Initialize() = 0;

	// Engine destruction.
	virtual void Shutdown() = 0;

	// Engine main loop.
	virtual int Run() = 0;

};