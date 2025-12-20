#pragma once

#include "svDef.h"

#pragma pack(push,8)

class SVCOLOR_API scColorTransform 
{
public:
	scColorTransform();
	~scColorTransform();

	bool RGB2LAB(double R, double G,  double B, double& dL, double& dA,  double& dB);
	bool RGB2LCH(double R, double G,  double B, double& dL, double& dC,  double& dH);

	bool SetODLut();
	bool RGB2OD(double R, double G,  double B, double& dC, double& dM,  double& dY);

	bool SetHueLut();
	bool RGB2HSI(double R, double G,  double B, double& dH, double& dS,  double& dI);
	bool RGB2HSL(double R, double G,  double B, double& dH, double& dS,  double& dL);

private:
	void	*m_hColorTransBase;
};

#pragma pack(pop)
