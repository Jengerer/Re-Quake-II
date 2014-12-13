#pragma once

#if !defined(Quake2CommonLibrary)
#if defined(Quake2CommonLibraryExport)
#define Quake2CommonLibrary __declspec(dllexport)
#else
#define Quake2CommonLibrary __declspec(dllimport)
#endif
#endif