#pragma once

#include "keyboard_shared.h"

// Keyboard managing class.
class KeyboardManager
{

public:

	KeyboardManager();

	// Reset keys to be unchanged.
	void ResetChanged();

	// Get keys current state.
	KeyState GetState(KeyCode key);

	// Modify key state.
	void UpdateKeyState(KeyCode key, KeyState newState);

private:

	KeyState states[EngineKeyMax];

};