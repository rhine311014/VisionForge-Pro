// vsAlignToolDef.cpp

#include "StdAfx.h"
#include "vsAlignToolDef.h"
#include "vsUVW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





// X1X2Y平台各轴绝对值转化为全局坐标系下各轴绝对值
//1.转化为算法中规定的方向；
//2.判断XXY安装方式，转换各连接点的位置（标准模型）
//3.计算
//4.根据XYY平台安装方式，将计算出的XYD的值转化为算法规定方向（X右Y上逆时针）
//5.将XYD从算法中的方向转化为平台坐标系的标准方向；

void CPlatformX1X2YInfo::ConvertX1X2YPltfmPosToXYDPltfmPos( double dX1, double dX2, double dY, double& dX0, double& dY0, double& dD0 )
{
	CX1X2Y platformX1X2Y;

	platformX1X2Y.SetX1X2YAxisCoordType(
		UVWAxisDirectType(m_nPlatformX1CoordType), 
		UVWAxisDirectType(m_nPlatformX2CoordType), 
		UVWAxisDirectType(m_nPlatformYCoordType));

	platformX1X2Y.SetX1X2YPos(
		sc2Vector(m_X1Pos.GetPosX(), m_X1Pos.GetPosY()),
		sc2Vector(m_X2Pos.GetPosX(), m_X2Pos.GetPosY()),
		sc2Vector(m_YPos.GetPosX(), m_YPos.GetPosY()));

	platformX1X2Y.SetSliderType(UVWSliderType(m_nPlatformUVWSliderType));

	platformX1X2Y.ConvertX1X2YToXYD(dX1, dX2, dY, dX0, dY0, dD0);
}


//1.将输入的XXY各基准位置(左手系)转化到标准全局坐标系下（右手系）
//2.判断XXY安装方式，转换各连接点的位置（标准模型）
//3.根据XXY安装方式，将输入的XYD（实际位置）转化为算法方向坐标（右旋90°）
//4.计算
//5.将XXY的值转化为实际方向
void CPlatformX1X2YInfo::ConvertXYDPltfmPosToX1X2YPltfmPos( double dX0, double dY0, double dD0, double& dX1, double& dX2, double& dY )
{
	CX1X2Y platformX1X2Y;

	platformX1X2Y.SetX1X2YAxisCoordType(
		UVWAxisDirectType(m_nPlatformX1CoordType), 
		UVWAxisDirectType(m_nPlatformX2CoordType), 
		UVWAxisDirectType(m_nPlatformYCoordType));

	platformX1X2Y.SetX1X2YPos(
		sc2Vector(m_X1Pos.GetPosX(), m_X1Pos.GetPosY()),
		sc2Vector(m_X2Pos.GetPosX(), m_X2Pos.GetPosY()),
		sc2Vector(m_YPos.GetPosX(), m_YPos.GetPosY()));

	platformX1X2Y.SetSliderType(UVWSliderType(m_nPlatformUVWSliderType));

	platformX1X2Y.ConvertXYDToX1X2Y(dX0, dY0, dD0, dX1, dX2, dY);
}



//XYY平台

//1.判断XYY的各轴方向，并根据方向将各轴的绝对位置转化为算法中规定的位置（Y向上为正，X向右为正）
//2.计算（得到的XYD值是X向右为正，Y向上为正）
//3.转化为标准坐标系的方向（左手系：X向右为正，Y向下为正）
void CPlatformXY1Y2Info::ConvertXY1Y2PltfmPosToXYDPltfmPos( double dY1, double dY2, double dX, double& dX0, double& dY0, double& dD0 )
{
	CXY1Y2 platformXY1Y2;

	platformXY1Y2.SetXY1Y2AxisCoordType(
		UVWAxisDirectType(m_nPlatformY1CoordType), 
		UVWAxisDirectType(m_nPlatformY2CoordType),
		UVWAxisDirectType(m_nPlatformXCoordType));

	platformXY1Y2.SetXY1Y2Pos(
		sc2Vector(m_Y1Pos.GetPosX(), m_Y1Pos.GetPosY()),
		sc2Vector(m_Y2Pos.GetPosX(), m_Y2Pos.GetPosY()),
		sc2Vector(m_XPos.GetPosX(), m_XPos.GetPosY()));

	platformXY1Y2.SetSliderType(UVWSliderType(m_nPlatformUVWSliderType));

	platformXY1Y2.ConvertXY1Y2ToXYD(dY1, dY2, dX, dX0, dY0, dD0);

}



// 将全局坐标系下各轴转化为XYY平台中各轴的绝对值//X0,Y0,D0是计算得到的，是XYD坐标系的标准方向(左手系)

//1.将XYD转化为算法中的方向（右手系）
//2.转化为XYY轴的位置
//3.将XYY算法中的方向转化为实际方向
void CPlatformXY1Y2Info::ConvertXYDPltfmPosToXY1Y2PltfmPos( double dX0, double dY0, double dD0, double& dY1, double& dY2, double& dX )
{

	CXY1Y2 platformXY1Y2;

	platformXY1Y2.SetXY1Y2AxisCoordType(
		UVWAxisDirectType(m_nPlatformY1CoordType), 
		UVWAxisDirectType(m_nPlatformY2CoordType),
		UVWAxisDirectType(m_nPlatformXCoordType));

	platformXY1Y2.SetXY1Y2Pos(
		sc2Vector(m_Y1Pos.GetPosX(), m_Y1Pos.GetPosY()),
		sc2Vector(m_Y2Pos.GetPosX(), m_Y2Pos.GetPosY()),
		sc2Vector(m_XPos.GetPosX(), m_XPos.GetPosY()));

	platformXY1Y2.SetSliderType(UVWSliderType(m_nPlatformUVWSliderType));

	platformXY1Y2.ConvertXYDToXY1Y2(dX0, dY0, dD0, dY1, dY2, dX);

}



//20160510增加对角补偿
bool AngleOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, int nPosMode, scDegree& angleOffset)
{
	if ( (nPosMode < 0) || (nPosMode > 1) )
	{
		return false;
	}

	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) ||(dTargetL2 <= 0) ||(dTargetW2 <= 0) )
	{
		return false;
	}

	sc2Vector objectP1, objectP2, targetP1, targetP2;
	if (nPosMode == 0)
	{
		objectP1 = sc2Vector(-0.5 * dObjectL1, -0.5 * dObjectW1);
		objectP2 = sc2Vector( 0.5 * dObjectL1,  0.5 * dObjectW1);
		targetP1 = sc2Vector(-0.5 * dTargetL2, -0.5 * dTargetW2);
		targetP2 = sc2Vector( 0.5 * dTargetL2,  0.5 * dTargetW2);	
	}
	else if (nPosMode == 1)
	{
		objectP1 = sc2Vector( 0.5 * dObjectL1, -0.5 * dObjectW1);
		objectP2 = sc2Vector(-0.5 * dObjectL1,  0.5 * dObjectW1);
		targetP1 = sc2Vector( 0.5 * dTargetL2, -0.5 * dTargetW2);
		targetP2 = sc2Vector(-0.5 * dTargetL2,  0.5 * dTargetW2);	
	}

	scLine objectLine, targetLine;
	objectLine.SetFromStartEnd(objectP1, objectP2);
	targetLine.SetFromStartEnd(targetP1, targetP2);

	scRadian radianAngle;
	sfAngleLine2Line(objectLine, targetLine, radianAngle);

	angleOffset = scDegree(radianAngle);
	angleOffset.SignedNormMod180();

	return true;
}

//20160510增加对角补偿
bool XYOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, double dF1, double dF2, int nPosMode, double& dOffsetX, double& dOffsetY)
{
	if ( (nPosMode < 0) || (nPosMode > 1) )
	{
		return false;
	}

	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) ||(dTargetL2 <= 0) ||(dTargetW2 <= 0) )
	{
		return false;
	}

	double dLeft = dF1;
	double dRight = dTargetL2 - dObjectL1 - dF1;
	dOffsetX = (dRight - dLeft)*0.5;

	double dTop = dF2;
	double dBottom = dTargetW2 - dObjectW1 - dF2;
	dOffsetY = (dBottom - dTop)*0.5;

	return true;
}

//20160510增加对角补偿
bool AngleDiagonalCalculate(double dObjectL1, double dObjectW1, int nPosMode, scDegree& angleDiagonal)
{
	if ( (nPosMode < 0) || (nPosMode > 1) )
	{
		return false;
	}

	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) )
	{
		return false;
	}

	sc2Vector objectP1, objectP2;
	if (nPosMode == 0)
	{
		objectP1 = sc2Vector(-0.5 * dObjectL1, -0.5 * dObjectW1);
		objectP2 = sc2Vector( 0.5 * dObjectL1,  0.5 * dObjectW1);
	}
	else if (nPosMode == 1)
	{
		objectP1 = sc2Vector( 0.5 * dObjectL1, -0.5 * dObjectW1);
		objectP2 = sc2Vector(-0.5 * dObjectL1,  0.5 * dObjectW1);
	}

	scLine objectLine;
	objectLine.SetFromStartEnd(objectP1, objectP2);

	angleDiagonal = scDegree(objectLine.GetRotation()).SignedNormMod180();

	return true;
}