#pragma once

#include "opengl_define.h"
#include "renderer/renderer_interface.h"
#include "renderer/resources_interface.h"

// Function to retrieve base renderer functions.
OpenGLLibrary Renderer::Interface *GetRendererInterface();

// Function to retrieve renderer shader utilties.
OpenGLLibrary Renderer::Resources *GetRendererResources();