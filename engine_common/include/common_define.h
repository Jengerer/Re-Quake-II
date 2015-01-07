#pragma once

#if !defined(CommonLibrary)
    #if defined(_WIN32)
        #if defined(CommonLibraryExport)
            #define CommonLibrary __declspec(dllexport)
        #else
            #define CommonLibrary __declspec(dllimport)
        #endif
    #else
        #define CommonLibrary
    #endif
#endif
