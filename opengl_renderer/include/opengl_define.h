#pragma once

#if !defined(OpenGLLibrary)
#if defined(OpenGLLibraryExport)
#define OpenGLLibrary __declspec(dllexport)
#else
#define OpenGLLibrary __declspec(dllimport)
#endif
#endif