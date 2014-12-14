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
	virtual ~Window();

	// Window modification functions.
	virtual void ResizeWindow(int width, int height);
	virtual bool UpdateFlags(WindowFlags newFlags);

	// Mouse functions.
	virtual void GetMousePosition(int *x, int *y) const = 0;
	virtual void SetMousePosition(int x, int y) = 0;

	// Switch to next frame buffer and present.
	virtual void SwapBuffers() = 0;

	// Keyboard device functions.
	WindowEventResult HandleKeyPress(KeyCode key);
	WindowEventResult HandleKeyRelease(KeyCode key);

	// Fill out current window size.
	inline void GetSize(int *width, int *height) {
		*width = this->width;
		*height = this->height;
	}
	inline WindowFlags GetFlags() const { return flags; }
	inline void SetInputListener(InputListener *listener) { this->listener = listener; }

private:

	// Helper for translating result enumerations.
	static WindowEventResult TranslateInputResult(InputEventResult result);

protected:

	int width;
	int height;
	WindowFlags flags;
	InputListener *listener;

};
