#pragma once

#ifdef SVIMAGEANALYSIS_EXPORTS
#define SVIMAGEANALYSIS_API __declspec(dllexport)
#else
#define SVIMAGEANALYSIS_API __declspec(dllimport)
#endif
