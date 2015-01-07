#pragma once

#if !defined(Quake2CommonLibrary)
    #if defined(_WIN32)
        #if defined(Quake2CommonLibraryExport)
            #define Quake2CommonLibrary __declspec(dllexport)
        #else
            #define Quake2CommonLibrary __declspec(dllimport)
        #endif
    #else
        #define Quake2CommonLibrary
    #endif
#endif
