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
	bool Create(const char *title, int width, int height, WindowFlags flags);
	void ResizeWindow(int width, int height);
	bool UpdateFlags(WindowFlags newFlags);

	// Handle events pending for this window.
	WindowEventResult HandleEvents();

	// Display the next frame.
	void SwapBuffers();

	// Get window dimensions.
	void GetSize(int *width, int *height) const;

	// Get cursor position.
	void GetMousePosition(int *x, int *y) const;

	// Set cursor position.
	void SetMousePosition(int x, int y);	

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