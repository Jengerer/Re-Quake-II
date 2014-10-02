#pragma once

#include "input_listener.h"

// Engine window flag structure.
union WindowFlags
{
	struct {
		char fullscreen : 1;
		char borderless : 1;
		char padding : 6;
	};
	char asCharacter;
};

// Enumeration for handling window events.
enum WindowEventResult
{
	WindowEventSuccess,
	WindowEventQuit,
	WindowEventError
};

// Base window structure.
class Window
{

public:

	Window(int width, int height, WindowFlags flags, InputListener *listener);
	virtual ~Window();

// Window parameter retrieval.

	inline int GetWidth() const;
	inline int GetHeight() const;
	inline WindowFlags GetFlags() const;

// Window event handling.

	bool HandleKeyPress(KeyCode key);
	bool HandleKeyRelease(KeyCode key);

protected:

	int width, height;
	WindowFlags flags;
	InputListener *listener;

};

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

WindowFlags Window::GetFlags() const
{
	return flags;
}