//vsAlignBaseDef.h
#pragma once

#ifdef VS_ALIGNBASE_EXPORTS
#define VS_ALIGNBASE_API _declspec(dllexport)
#else
#define VS_ALIGNBASE_API _declspec(dllimport)
#endif