#pragma once

#if !defined(ClientLibrary)
    #if defined(_WIN32)
        #if defined(ClientLibraryExport)
            #define ClientLibrary __declspec(dllexport)
        #else
            #define ClientLibrary __declspec(dllimport)
        #endif
    #else
        #define ClientLibrary
    #endif
#endif
