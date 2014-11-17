#pragma once

#include "keyboard_shared.h"

// Input event return values.
enum InputEventResult
{
	InputEventSuccess,
	InputEventQuit,
	InputEventError
};

// Input event interface.
class InputListener
{

public:

	// Keyboard event handlers.
	virtual InputEventResult OnKeyPress(KeyCode key) = 0;
	virtual InputEventResult OnKeyRelease(KeyCode key) = 0;

};