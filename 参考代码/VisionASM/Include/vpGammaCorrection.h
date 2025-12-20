#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

#define ColorNumber 256
#define GammaParaMin 0
#define GammaParaMax 10

class SVCORE_API cpGammaCorrection
{
public:
	//构造函数
	cpGammaCorrection(void);

	//析构函数
	~cpGammaCorrection(void);

	//设置/获取Gamma参数(范围0~10.0)
	bool SetGammaPara(double dGammaPara);
	double GetGammaPara();

	//执行Gamma校正
	bool ExecuteGammaCorrection(scImage8& src);
	bool ExecuteGammaCorrection(scImage24& src);
	bool ExecuteGammaCorrection(cpImage& src);

private:
	double m_dGammaPara;
	unsigned char m_GammaLUT[ColorNumber];
};

#pragma pack(pop)