#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svImage.h"
#include "svColorCalibration.h"

#pragma pack(push,8)

class SVCOLOR_API scColorDifference  
{
public:
	scColorDifference();
	~scColorDifference();

	bool	SetCalibrateResult(scColorCalibrationResult& result);

	bool	SetFastCTLut();	

	bool	ColorTrain(const scImage24& image, scRect roi, bool bQuickMode);
	bool	ColorTrain(const scImage32& image, scRect roi, bool bQuickMode);

	bool	CalculateColorDifference(const scImage24& image, scRect roi, double& dDE, bool bCMC=false);
	bool	CalculateColorDifference(const scImage32& image, scRect roi, double& dDE, bool bCMC=false);
	bool	CalculateColorDifference(double R, double G, double B, double& dDE, bool bCMC=false);

	double	GetModelL() ;
	double	GetModelA() ;
	double	GetModelB() ;
	double	GetModelC() ;
	double	GetModelH() ;

	double	GetProductL() ;
	double	GetProductA() ;
	double	GetProductB() ;
	double	GetProductC() ;
	double	GetProductH() ;

	double	GetDL() ;
	double	GetDA() ;
	double	GetDB() ;
	double	GetDC() ;
	double	GetDH() ;

private:
	void    *m_hColorDiff;
};

#pragma pack(pop)
