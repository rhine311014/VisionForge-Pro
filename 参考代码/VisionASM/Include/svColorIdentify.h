#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svImage.h"
#include <vector>

#pragma pack(push,8)

class SVCOLOR_API scColorIdentify  
{
public:
	enum Precision
	{
		eCoarse = 0,		// 粗略模式
		eNormal,			// 一般模式
		eFine,			// 精细模式
	};

public:
	scColorIdentify();
	~scColorIdentify();

	void Reset();

	bool SetSaturationThre(double dSaturationThre);		

	bool SetIdentifyPrecicion(Precision nPrecicionType);

	bool AddSample(const scImage24& image, scRect roi);	
	bool AddSample(const scImage32& image, scRect roi);	

	bool ExcuteIdentify(const scImage24& image, scRect roi, int& nMatchSampleID, double& dMatchScore, svStd vector<double>& score);
	bool ExcuteIdentify(const scImage32& image, scRect roi, int& nMatchSampleID, double& dMatchScore, svStd vector<double>& score);

	int  GetSampleNums();
	int  GetSampleBinNums();
	bool GetSampleSpectrum(int nSampleID, svStd vector<double>& spectrum);
	bool GetProductSpectrum(svStd vector<double>& spectrum);

private:	
	void    *m_hColorIdt;
};

#pragma pack(pop)
