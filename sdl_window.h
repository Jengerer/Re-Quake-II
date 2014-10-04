#pragma once

#include "window.h"
#include <SDL.h>
#include <SDL_opengl.h>

// SDL window implementation.
class SDLWindow : public Window
{

public:

	SDLWindow(InputListener *listener);
	~SDLWindow();

	// Initialize modules needed to create/update window.
	bool Initialize();

	// Create/update window size/flags.
	bool Update(int width, int height, WindowFlags flags);

	// Handle events pending for this window.
	WindowEventResult HandleEvents();

	// Display the next frame.
	void SwapBuffers();

private:

	// Helper for dispatching a keyboard press event.
	WindowEventResult HandleKeyPress(const SDL_KeyboardEvent *event);

	// Helper for dispatching a keyboard release event.
	WindowEventResult HandleKeyRelease(const SDL_KeyboardEvent *event);

	// Translate key from SDL code to engine code.
	static KeyCode TranslateSDLKey(SDL_Keycode sdlCode);

private:

	// Constant for window buffer depth.
	static const int BufferDepth = 24;

private:

	SDL_Window *sdlWindow;
	SDL_GLContext *glContext;

};