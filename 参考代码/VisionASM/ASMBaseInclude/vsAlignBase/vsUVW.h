//vsUVW.h
#pragma once
#include "svVector.h"
#include "vsAlignBaseDef.h"

#pragma pack(push,8)

// 坐标轴的方向
// X1轴方向，向右为正，向左为负
// X2轴方向：向右为正，向左为负
// Y轴方向：向上为正，向下为负
typedef enum tagUVWAxisDirectType
{
	eUVWAxisDirectNegative = -1,			//  反向，为负
	eUVWAxisDirectPositive = 1				//  同向，为正
}UVWAxisDirectType;

// UVW滑块类型
typedef enum enumUVWSliderType
{
	eUVWFixedSlider		= 0,			//  固定滑块，类型1，转动辅链接在下面的轴上
	eUVWShiftSlider		= 1				//	移动滑块，类型2，转动辅链接找上面的平台上
}UVWSliderType; 

// X1X2Y平台类型信息
class VS_ALIGNBASE_API CX1X2Y
{	
public:
	CX1X2Y()
	{
		// 平台信息		
		m_nX1CoordType = eUVWAxisDirectPositive;			// X1轴方向，向右为正，向左为负
		m_nX2CoordType = eUVWAxisDirectPositive;			// X2轴方向：向右为正，向左为负
		m_nYCoordType  = eUVWAxisDirectPositive;			// Y轴方向：向上为正，向下为负

		m_X1Pos = sc2Vector(67.5, -57);		
		m_X2Pos = sc2Vector(67.5, 57);
		m_YPos = sc2Vector(-57, 67.5);

		m_nUVWSliderType = eUVWFixedSlider;
	}
	
	virtual ~CX1X2Y(){}
	
public:
	void	SetX1X2YPos(sc2Vector X1pos, sc2Vector X2pos, sc2Vector Ypos)
	{
		m_X1Pos = X1pos; m_X2Pos = X2pos; m_YPos = Ypos;
	}
	
	void	GetX1X2YPos(sc2Vector& X1pos, sc2Vector& X2pos, sc2Vector& Ypos)
	{	
		X1pos = m_X1Pos; X2pos = m_X2Pos; Ypos = m_YPos;	
	}

	sc2Vector  GetX1Pos(){return m_X1Pos;}
	sc2Vector  GetX2Pos(){return m_X2Pos;}
	sc2Vector  GetYPos(){return m_YPos;}


	void	SetX1X2YAxisCoordType(UVWAxisDirectType actX1, UVWAxisDirectType actX2, UVWAxisDirectType actY)
	{
		m_nX1CoordType = actX1; m_nX2CoordType = actX2; m_nYCoordType = actY;
	}
	
	void	GetX1X2YAxisCoordType(UVWAxisDirectType& actX1, UVWAxisDirectType& actX2, UVWAxisDirectType& actY)
	{	
		actX1 = m_nX1CoordType; actX2 = m_nX2CoordType; actY = m_nYCoordType;	
	}

	void	SetSliderType(UVWSliderType nUVWSliderType)
	{
		m_nUVWSliderType = nUVWSliderType;
	}

	UVWSliderType GetSliderType()
	{
		return m_nUVWSliderType;
	}

	void ConvertX1X2YToXYD(double dX1, double dX2, double dY, double& dX0, double& dY0, double& dD0);
	void ConvertXYDToX1X2Y(double dX0, double dY0, double dD0, double& dX1, double& dX2, double& dY);
	
public:
	// 平台信息
	UVWAxisDirectType		m_nX1CoordType;		// X1轴坐标方向：向右为正，向左为负
	UVWAxisDirectType		m_nX2CoordType;		// X2轴坐标方向：向右为正，向左为负
	UVWAxisDirectType		m_nYCoordType;		// Y轴坐标方向：向上为正，向下为负
	
	sc2Vector  m_X1Pos;							// X1X2Y平台共享参数：X1X2Y坐标位置
	sc2Vector  m_X2Pos;
	sc2Vector  m_YPos;

	UVWSliderType			m_nUVWSliderType;	// UVW平台安装结构类型:类型1、类型2？
	
};




// XY1Y2平台类型信息
class VS_ALIGNBASE_API CXY1Y2
{	
public:
	CXY1Y2()
	{
		// 平台信息		
		m_nY1CoordType = eUVWAxisDirectPositive;;			// Y1轴方向，向上为正，向下为负
		m_nY2CoordType = eUVWAxisDirectPositive;;			// Y2轴方向：向上为正，向下为负
		m_nXCoordType  = eUVWAxisDirectPositive;			// X轴方向：向右为正，向左为负

		m_Y1Pos = sc2Vector(-78.5, 79);		
		m_Y2Pos = sc2Vector(78.5, 79);
		m_XPos = sc2Vector(70, -78.5);

		m_nUVWSliderType = eUVWFixedSlider;
	}
	
	virtual ~CXY1Y2(){}
	
public:	
	void	SetXY1Y2Pos(sc2Vector Y1Pos, sc2Vector Y2Pos, sc2Vector XPos)
	{
		m_Y1Pos = Y1Pos; m_Y2Pos = Y2Pos; m_XPos = XPos;
	}	
	void	GetXY1Y2Pos(sc2Vector& Y1Pos, sc2Vector& Y2Pos, sc2Vector& XPos)
	{
		Y1Pos = m_Y1Pos; Y2Pos = m_Y2Pos; XPos = m_XPos;
	}
	sc2Vector  GetY1Pos(){return m_Y1Pos;}
	sc2Vector  GetY2Pos(){return m_Y2Pos;}
	sc2Vector  GetXPos(){return m_XPos;}

	void	SetXY1Y2AxisCoordType(UVWAxisDirectType actY1, UVWAxisDirectType actY2, UVWAxisDirectType actX)
	{
		m_nY1CoordType = actY1; m_nY2CoordType = actY2; m_nXCoordType = actX;
	}	
	void	GetXY1Y2AxisCoordType(UVWAxisDirectType& actY1, UVWAxisDirectType& actY2, UVWAxisDirectType& actX)
	{
		actY1 = m_nY1CoordType; actY2 = m_nY2CoordType; actX = m_nXCoordType;
	}

	void	SetSliderType(UVWSliderType nUVWSliderType)
	{
		m_nUVWSliderType = nUVWSliderType;
	}

	UVWSliderType GetSliderType()
	{
		return m_nUVWSliderType;
	}

	void ConvertXY1Y2ToXYD(double dY1, double dY2, double dX, double& dX0, double& dY0, double& dD0);
	void ConvertXYDToXY1Y2(double dX0, double dY0, double dD0, double& dY1, double& dY2, double& dX);
	
public:
	// 平台信息
	UVWAxisDirectType		m_nY1CoordType;		// Y1轴坐标方向：向上为正，向下为负
	UVWAxisDirectType		m_nY2CoordType;		// Y2轴坐标方向：向上为正，向下为负
	UVWAxisDirectType		m_nXCoordType;		// X轴坐标方向：向左为正，向右为负
	
	sc2Vector  m_Y1Pos;							// XY1Y2平台共享参数：XY1Y2各点位置(>0)
	sc2Vector  m_Y2Pos;
	sc2Vector  m_XPos;

	UVWSliderType			m_nUVWSliderType;	// UVW平台安装结构类型:类型1、类型2？	
};

#pragma pack(pop)