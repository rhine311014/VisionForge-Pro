#pragma once

#ifdef SVDXFFILE_EXPORTS
#define SVDXFFILE_API __declspec(dllexport)
#else
#define SVDXFFILE_API __declspec(dllimport)
#endif
