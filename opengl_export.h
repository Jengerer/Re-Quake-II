#pragma once

#include "shared_defines.h"
#include "renderer.h"
#include "renderer_resources.h"

// Function to retrieve base renderer functions.
LibraryExport Renderer::Interface *GetRendererInterface();

// Function to retrieve renderer shader utilties.
LibraryExport Renderer::Resources *GetRendererResources();