#pragma once

#if !defined(ClientLibrary)
#if defined(ClientLibraryExport)
#define ClientLibrary __declspec(dllexport)
#else
#define ClientLibrary __declspec(dllimport)
#endif
#endif