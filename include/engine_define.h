#pragma once

#if !defined(EngineLibrary)
#if defined(EngineLibraryExport)
#define EngineLibrary __declspec(dllexport)
#else
#define EngineLibrary __declspec(dllimport)
#endif
#endif