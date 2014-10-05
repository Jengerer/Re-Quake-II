#include "keyboard_manager.h"

// Clear all key states.
KeyboardManager::KeyboardManager()
{
	for (int i = 0; i < EngineKeyMax; ++i) {
		states[i].raw = 0;
	}
}

// Reset changed flag for a new frame.
void KeyboardManager::ResetChanged()
{
	for (int i = 0; i < EngineKeyMax; ++i) {
		states[i].bits.changed = 0;
	}
}

// Get current state.
KeyState KeyboardManager::GetState(KeyCode key)
{
	return states[key];
}

// Update key with new state.
void KeyboardManager::UpdateKeyState(KeyCode key, KeyState newState)
{
	states[key] = newState;
}
