#pragma once

#if !defined(EngineLibrary)
    #if defined(_WIN32)
        #if defined(EngineLibraryExport)
            #define EngineLibrary __declspec(dllexport)
        #else
            #define EngineLibrary __declspec(dllimport)
        #endif
    #else
        #define EngineLibrary
    #endif
#endif
