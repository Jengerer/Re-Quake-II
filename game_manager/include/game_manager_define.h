#pragma once

#if !defined(GameManagerLibrary)
    #if defined(_WIN32)
        #if defined(GameManagerLibraryExport)
            #define GameManagerLibrary __declspec(dllexport)
        #else
            #define GameManagerLibrary __declspec(dllimport)
        #endif
    #else
        #define GameManagerLibrary
    #endif
#endif
