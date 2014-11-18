#pragma once

#if !defined(CommonLibrary)
#if defined(CommonLibraryExport)
#define CommonLibrary __declspec(dllexport)
#else
#define CommonLibrary __declspec(dllimport)
#endif
#endif