#pragma once

#include "input_listener.h"
#include "window_shared.h"

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

	Window();

	// Update the listener to pass input events to.
	void SetInputListener(InputListener *listener);

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

WindowFlags Window::GetFlags() const
{
	return flags;
}