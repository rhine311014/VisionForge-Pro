#pragma once

#include "svDef.h"

#pragma pack(push,8)

class SVCORE_API scTimer
{
public:
	scTimer();
	void    Reset();
	float   GetTime(BOOL bReset);													//单位：  秒
	float   GetTimeMilli(BOOL bReset)   { return GetTime(bReset) * 1000.0f; }		//单位：毫秒
	float   GetTimeMicro(BOOL bReset)	{ return GetTime(bReset) * 1000000.0f; }	//单位：微秒

protected:
	LONGLONG m_CounterFrequency;
	LONGLONG m_ResetTime;
};

#pragma pack(pop)
