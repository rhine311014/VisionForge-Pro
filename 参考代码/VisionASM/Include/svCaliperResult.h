#pragma once

#include "svDef.h"

#pragma pack(push,8)

class SVCORE_API scCaliperOneEdgeResult
{
public:
	scCaliperOneEdgeResult();

public:
	double	m_dScore;
	double	m_dEdgePointX;
	double	m_dEdgePointY;
	double	m_dOffset;
	double	m_dContrast;
};

class SVCORE_API scCaliperTwoEdgeResult
{
public:
	scCaliperTwoEdgeResult();

public:
	double	m_dScore;
	double	m_dCenterPointX;
	double	m_dCenterPointY;
	double	m_dCenterOffset;
	double	m_dTwoEdgeWidth;

	double	m_dEdgePoint1X;
	double	m_dEdgePoint1Y;
	double	m_dEdgePoint1Offset;
	double	m_dEdgePoint1Contrast;

	double	m_dEdgePoint2X;
	double	m_dEdgePoint2Y;
	double	m_dEdgePoint2Offset;
	double	m_dEdgePoint2Contrast;
};

#pragma pack(pop)
