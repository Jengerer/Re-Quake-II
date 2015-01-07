#pragma once

#if !defined(OpenGLLibrary)
    #if defined(_WIN32)
        #if defined(OpenGLLibraryExport)
            #define OpenGLLibrary __declspec(dllexport)
        #else
            #define OpenGLLibrary __declspec(dllimport)
        #endif
    #else
        #define OpenGLLibrary
    #endif
#endif
