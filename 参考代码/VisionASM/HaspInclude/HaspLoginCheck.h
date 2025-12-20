#pragma once

#ifdef HASPLOGINCHECK_EXPORTS
#define HASPLOGINCHECK_API __declspec(dllexport)
#else
#define HASPLOGINCHECK_API __declspec(dllimport)
#endif

HASPLOGINCHECK_API bool CheckDongle(unsigned int nFeatureId = 0);
HASPLOGINCHECK_API bool CheckDongle_13(unsigned int nFeatureId = 0);
HASPLOGINCHECK_API bool CheckDongle_78(unsigned int nFeatureId = 0);