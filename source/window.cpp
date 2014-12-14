#include "window.h"
#include <error_stack.h>

Window::Window()
{
	// Reset flags.
	flags.raw = 0;
}

Window::~Window()
{
}

void Window::ResizeWindow(int width, int height)
{
	this->width = width;
	this->height = height;
}

bool Window::UpdateFlags(WindowFlags newFlags)
{
	this->flags = newFlags;
	return true;
}

// Pass keyboard event to the listener and convert result.
WindowEventResult Window::HandleKeyPress(KeyCode key)
{
	InputEventResult result = listener->OnKeyPress(key);
	return TranslateInputResult(result);
}

// Pass keyboard event to the listener and convert result.
WindowEventResult Window::HandleKeyRelease(KeyCode key)
{
	InputEventResult result = listener->OnKeyRelease(key);
	return TranslateInputResult(result);
}

// Translate an input event result to a window event result.
WindowEventResult Window::TranslateInputResult(InputEventResult result)
{
	switch (result) {
	case InputEventSuccess:
		return WindowEventSuccess;
	case InputEventQuit:
		return WindowEventQuit;
	case InputEventError:
		return WindowEventError;
	default:
		ErrorStack::Log("Invalid input event result received.\n");
		return WindowEventError;
	}
}
