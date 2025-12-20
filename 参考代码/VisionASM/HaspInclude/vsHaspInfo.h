#pragma once

#ifdef HaspCheck_EXPORTS
#define HaspCheck_API __declspec(dllexport)
#else
#define HaspCheck_API __declspec(dllimport)
#endif

HaspCheck_API BOOL GetDongleID(CString& strDongleID);