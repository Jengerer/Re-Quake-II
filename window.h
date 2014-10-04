#pragma once

#include "input_listener.h"

// Engine window flag structure.
union WindowFlags
{
	struct {
		char fullscreen : 1;
		char borderless : 1;
		char verticalSync : 1;
	} bits;
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

	Window(InputListener *listener);

	// Get display flags for this window.
	inline WindowFlags GetFlags() const;

	// Handle and dispatch keyboard press event.
	WindowEventResult HandleKeyPress(KeyCode key);

	// Handle and dispatch keyboard release event.
	WindowEventResult HandleKeyRelease(KeyCode key);

private:

	// Helper for translating result enumerations.
	static WindowEventResult TranslateInputResult(InputEventResult result);

protected:

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