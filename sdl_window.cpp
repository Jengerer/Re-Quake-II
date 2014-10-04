#include "sdl_window.h"
#include "error_stack.h"
#include <stdio.h>

SDLWindow::SDLWindow()
{
}

SDLWindow::~SDLWindow()
{
	// Destroy context/window.
	if (glContext != nullptr) {
		SDL_GL_DeleteContext(glContext);
	}
	if (sdlWindow != nullptr) {
		SDL_DestroyWindow(sdlWindow);
	}
}

// Initialize SDL window with OpenGL context.
bool SDLWindow::Initialize()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ErrorStack::Log("Failed to initialize SDL: %s.\n", SDL_GetError());
        return false;
    }

	// Set GL version.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Set buffering attributes.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, BufferDepth);
    return true;
}

// Create window for specific size and flags.
bool SDLWindow::Create(const char *title, int width, int height, WindowFlags flags)
{
	// Create window if there doesn't exist one.
	Uint32 sdlFlags = TranslateWindowFlags(flags);
	SDL_Window *sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlFlags | SDL_WINDOW_OPENGL);
	if (sdlWindow == nullptr) {
        ErrorStack::Log("Failed to create SDL window.\n");
        return false;
    }
	this->sdlWindow = sdlWindow;

    // Create OpenGL context.
    SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
    if (glContext == nullptr) {
        ErrorStack::Log("Failed to create OpenGL context: %s.\n", SDL_GetError());
        return false;
    }
	this->glContext = glContext;

	// Enable vertical sync if it's flagged.
	if (flags.bits.verticalSync) {
		SDL_GL_SetSwapInterval(1);
	}
	return true;
}

// Change window size.
void SDLWindow::ResizeWindow(int width, int height)
{
	// TODO: in full-screen does this work?
	SDL_SetWindowSize(sdlWindow, width, height);
}

// Update window flags.
bool SDLWindow::UpdateFlags(WindowFlags newFlags)
{
	// Update borderless.
	if (flags.bits.borderless != newFlags.bits.borderless) {
		SDL_bool sdlBorderless = (newFlags.bits.borderless ? SDL_TRUE : SDL_FALSE);
		SDL_SetWindowBordered(sdlWindow, sdlBorderless);
	}
	// Update fullscreen.
	if (flags.bits.fullscreen != newFlags.bits.fullscreen) {
		Uint32 fullscreenFlags = (newFlags.bits.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (SDL_SetWindowFullscreen(sdlWindow, fullscreenFlags) != 0) {
			ErrorStack::Log("Failed to set window fullscreen.\n");
			return false;
		}
	}
	// Update vertical sync.
	if (flags.bits.verticalSync != newFlags.bits.verticalSync) {
		int swapValue = (newFlags.bits.verticalSync ? 1 : 0);
		if (SDL_GL_SetSwapInterval(swapValue) != 0) {
			ErrorStack::Log("Failed to enable vertical-sync.\n");
			return false;
		}
	}
	return true;
}

// Handle window events.
WindowEventResult SDLWindow::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return WindowEventQuit;
		case SDL_KEYDOWN:
			return HandleKeyPress(&event.key);
		case SDL_KEYUP:
			return HandleKeyRelease(&event.key);
		default:
			break;
		}
	}
	return WindowEventSuccess;
}

// Display the next frame in the window.
void SDLWindow::SwapBuffers()
{
	SDL_GL_SwapWindow(sdlWindow);
}

// Get the window size.
void SDLWindow::GetSize(int *width, int *height) const
{
	SDL_GetWindowSize(sdlWindow, width, height);
}

// Get the mouse cursor position.
void SDLWindow::GetMousePosition(int *x, int *y) const
{
	SDL_GetMouseState(x, y);
}

// Set mouse cursor position.
void SDLWindow::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(sdlWindow, x, y);
}

// Handle key press event for window.
WindowEventResult SDLWindow::HandleKeyPress(const SDL_KeyboardEvent *event)
{
	// Check if valid key.
	KeyCode engineKey = TranslateSDLKey(event->keysym.sym);
	if (engineKey == EngineKeyInvalid) {
		return WindowEventSuccess;
	}

	// Pass to base handler.
	return Window::HandleKeyPress(engineKey);
}

// Handle key press event for window.
WindowEventResult SDLWindow::HandleKeyRelease(const SDL_KeyboardEvent *event)
{
	// Check if valid key.
	KeyCode engineKey = TranslateSDLKey(event->keysym.sym);
	if (engineKey == EngineKeyInvalid) {
		return WindowEventSuccess;
	}

	// Pass to base handler.
	return Window::HandleKeyRelease(engineKey);
}

// Convert from SDL code to engine.
KeyCode SDLWindow::TranslateSDLKey(SDL_Keycode sdlCode)
{
	// Alphabet translation.
	if ((sdlCode >= SDLK_a) && (sdlCode <= SDLK_z)) {
		int engineValue = static_cast<int>(EngineKeyA) + (sdlCode - SDLK_a);
		return static_cast<KeyCode>(engineValue);
	}
	return static_cast<KeyCode>(EngineKeyInvalid);
}
