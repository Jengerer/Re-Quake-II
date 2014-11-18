#pragma once

#if !defined(GameManagerLibrary)
#if defined(GameManagerLibraryExport)
#define GameManagerLibrary __declspec(dllexport)
#else
#define GameManagerLibrary __declspec(dllimport)
#endif
#endif