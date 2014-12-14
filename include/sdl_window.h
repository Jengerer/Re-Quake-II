#pragma once

#include "window.h"
#include <SDL.h>
#include <SDL_opengl.h>

// SDL window implementation.
class SDLWindow : public Window
{

public:

	SDLWindow();
	~SDLWindow();

	// Initialize modules needed to create/update window.
	bool Initialize();

	// Create/update window size/flags.
	virtual bool Create(const char *title, int width, int height, WindowFlags flags);
	virtual void ResizeWindow(int width, int height);
	virtual bool UpdateFlags(WindowFlags newFlags);

	// Mouse functions.
	virtual void GetMousePosition(int *x, int *y) const;
	virtual void SetMousePosition(int x, int y);	

	// Display the next frame.
	virtual void SwapBuffers();

	// Poll for and handle pending events.
	WindowEventResult HandleEvents();

private:

	// Helper for dispatching a keyboard press event.
	WindowEventResult HandleKeyPress(const SDL_KeyboardEvent *event);

	// Helper for dispatching a keyboard release event.
	WindowEventResult HandleKeyRelease(const SDL_KeyboardEvent *event);

	// Translate key from SDL code to engine code.
	static KeyCode TranslateSDLKey(SDL_Keycode sdlCode);

	// Translate flags from window flags to SDL flags.
	static Uint32 TranslateWindowFlags(WindowFlags flags);

private:

	// Constant for window buffer depth.
	static const int BufferDepth = 24;

private:

	SDL_Window *sdlWindow;
	SDL_GLContext glContext;

};