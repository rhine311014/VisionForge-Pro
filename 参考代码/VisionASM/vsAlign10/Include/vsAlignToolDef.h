// vsAlignToolDef.h
#ifdef VS_ALIGN_EXPORTS
#define VS_ALIGN_API _declspec(dllexport)
#else
#define VS_ALIGN_API _declspec(dllimport)
#endif

#pragma once

#include <vector>
#include <afxtempl.h>
#include "svVector.h"
#include "svMatrix.h"
#include "svLine.h"
#include "svMeasure.h"
#include "sv2XformLinear.h"
#include "svAffineRect.h"
//#define VS_ALIGN_API

#define MAXPOSNUM			8
#define ALIGNOFFSETNUM		25
#define ALIGNBENCHOFFSETNUM		25
#define TARGETCANDIDATENUM	5
#define OBJECTCANDIDATENUM  5
#define CALIBOBJECTCANDIDATENUM  1
#define VIRTUALOBJECTCANDIDATENUM  1
#define CALIBSEPARATECAMMOVENUM 2
#define ALNCALIBTARGETCANDIDATENUM  5
#define INSPECTCANDIDATENUM	5

// 平台标定类型
typedef enum enumPlatformCalibType
{
	ePlatformCalibUnknown	= -1,			// 未知
	ePlatformCalib5Point	= 0,			// 5点基础标定（计算2X2旋转矩阵）
	ePlatformCalib9Point	= 1,			// 9点H矩阵（计算3X3旋转矩阵）
} PlatformCalibType;



typedef enum enumCoordPosType
{
	eCPTAllPos		= -2,			// 全部点
	eCPTAllAuxPos	= -1,			// 全部辅助点
	eCPTMainPos		= 0,			// 主点
	eCPTAuxPos1		= 1,			// 辅助点1
	eCPTAuxPos2		= 2,			// 辅助点2
	eCPTAuxPos3		= 3,			// 辅助点3
	eCPTAuxPos4		= 4,			// 辅助点4
} CoordPosType;

// 基本信息类型
// 平台坐标系的位置和姿态

// 在二维坐标系下的位置姿态数据结构(平台坐标)
#define MaxAuxiliaryPosNum	4	// 最大辅助位置数量
class VS_ALIGN_API CCoordPos
{
public:
	CCoordPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
// 		m_dWidth = 0;
// 		m_dHeight = 0;
		m_vdAuxiliaryPosX.resize(MaxAuxiliaryPosNum, 0);		// 辅助位置X
		m_vdAuxiliaryPosY.resize(MaxAuxiliaryPosNum, 0);		// 辅助位置Y
		m_vdAuxiliaryAngle.resize(MaxAuxiliaryPosNum, 0);		// 辅助旋转角度，单位度

		m_dLineAngle1 = 0;
		m_dLineAngle2 = 0;
        m_bFindSearchRect = false;
        m_bFindLine1 = false;
        m_bFindLine2 = false;
	}

	CCoordPos(const CCoordPos& coordPos)
	{
		m_dPosX = coordPos.m_dPosX;			// 位置X
		m_dPosY = coordPos.m_dPosY;			// 位置Y
		m_dAngle = coordPos.m_dAngle;			// 旋转角度，单位度

		m_vdAuxiliaryPosX = coordPos.m_vdAuxiliaryPosX;
		m_vdAuxiliaryPosY = coordPos.m_vdAuxiliaryPosY;
		m_vdAuxiliaryAngle = coordPos.m_vdAuxiliaryAngle;

		m_dLineAngle1 = coordPos.m_dLineAngle1;
		m_dLineAngle2 = coordPos.m_dLineAngle2;

		m_SearchRect = coordPos.m_SearchRect;
		m_bFindSearchRect = coordPos.m_bFindSearchRect;
		m_bFindLine1 = coordPos.m_bFindLine1;
		m_bFindLine2 = coordPos.m_bFindLine2;
	}

	CCoordPos(double dPosX, double dPosY, double dAngle, double dLineAngle1 = 0, double dLineAngle2 = 0)
	{
		m_dPosX = dPosX;
		m_dPosY = dPosY;
		m_dAngle = dAngle;
//		m_dWidth = dWidth;
//		m_dHeight = dHeight;
		m_vdAuxiliaryPosX.resize(MaxAuxiliaryPosNum, 0);		// 辅助位置X
		m_vdAuxiliaryPosY.resize(MaxAuxiliaryPosNum, 0);		// 辅助位置Y
		m_vdAuxiliaryAngle.resize(MaxAuxiliaryPosNum, 0);		// 辅助旋转角度，单位度

		m_dLineAngle1 = dLineAngle1;
		m_dLineAngle2 = dLineAngle2;

        m_bFindSearchRect = false;
        m_bFindLine1 = true;
        m_bFindLine2 = true;
	}
	BOOL operator==(const CCoordPos& pos)
	{
		bool bRet = true;
		if ((m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle))
		{
			bRet = false;
		}
//		if ((m_dWidth != pos.m_dWidth) || (m_dHeight != pos.m_dHeight))
//		{
//			bRet = false;
//		}
		if ((m_dLineAngle1 != pos.m_dLineAngle1) || (m_dLineAngle2 != pos.m_dLineAngle2))
		{
			bRet = false;
		}
        if ((m_bFindLine1 != pos.m_bFindLine1) || (m_bFindLine2 != pos.m_bFindLine2) || (m_bFindSearchRect != pos.m_bFindSearchRect))
        {
            bRet = false;
        }
		if (m_vdAuxiliaryPosX.size() != pos.m_vdAuxiliaryPosX.size() 
			|| m_vdAuxiliaryPosY.size() != pos.m_vdAuxiliaryPosY.size() 
			|| m_vdAuxiliaryAngle.size() != pos.m_vdAuxiliaryAngle.size() )
		{
			bRet = bRet && false;
		}
		int i = 0;
		for (i = 0; i < m_vdAuxiliaryPosX.size() && i < m_vdAuxiliaryPosY.size() && i < m_vdAuxiliaryAngle.size(); i++)
		{
			if (m_vdAuxiliaryPosX.at(i) != pos.m_vdAuxiliaryPosX.at(i)
				|| m_vdAuxiliaryPosY.at(i) != pos.m_vdAuxiliaryPosY.at(i)
				|| m_vdAuxiliaryAngle.at(i) != pos.m_vdAuxiliaryAngle.at(i))
			{
				bRet = bRet && false;
			}
		}
		return bRet;
	}
	BOOL operator!=(const CCoordPos& pos)
	{
		return !(*this == pos);
	}
	
public:
	double GetPosX(){ return m_dPosX; }
	double GetPosY(){ return m_dPosY; }
	double GetAngle(){ return m_dAngle; }
// 	double GetWidth(){ return m_dWidth; }
// 	double GetHeight(){return m_dHeight; }
	double GetAuxiliaryPosX(int nIndex){ return (nIndex >= 0 && nIndex < m_vdAuxiliaryPosX.size()) ? m_vdAuxiliaryPosX.at(nIndex) : 0; }
	double GetAuxiliaryPosY(int nIndex){ return (nIndex >= 0 && nIndex < m_vdAuxiliaryPosY.size()) ? m_vdAuxiliaryPosY.at(nIndex) : 0; }
	double GetAuxiliaryAngle(int nIndex){ return (nIndex >= 0 && nIndex < m_vdAuxiliaryAngle.size()) ? m_vdAuxiliaryAngle.at(nIndex) : 0; }
	double GetLineAngle1() {return m_dLineAngle1;}
	double GetLineAngle2() {return m_dLineAngle2;}


	sc2Vector GetPos() { return sc2Vector(m_dPosX, m_dPosY); }
	sc2Vector GetAuxPos(int nIndex) { return (nIndex >= 0 && nIndex < m_vdAuxiliaryPosX.size()) ? sc2Vector(m_vdAuxiliaryPosX.at(nIndex), m_vdAuxiliaryPosY.at(nIndex)) : sc2Vector(0,0); }

	void   SetPos(sc2Vector vPos) { m_dPosX = vPos.GetX(); m_dPosY = vPos.GetY(); };
	void   SetPos(double dPosX, double dPosY) { m_dPosX = dPosX; m_dPosY = dPosY; };

	void   SetPosAngle(sc2Vector vPos, double dAngle){ m_dPosX = vPos.GetX(); m_dPosY = vPos.GetY(); m_dAngle = dAngle;};
	void   SetPosAngle(double dPosX, double dPosY, double dAngle){m_dPosX = dPosX; m_dPosY = dPosY; m_dAngle = dAngle;};

	BOOL   SetAuxPos(int nIndex, sc2Vector vPos) 
	{ 
		if (nIndex < 0 || nIndex > m_vdAuxiliaryPosX.size() || nIndex > m_vdAuxiliaryPosY.size())
		{
			return FALSE;
		}

		m_vdAuxiliaryPosX[nIndex] = vPos.GetX(); 
		m_vdAuxiliaryPosY[nIndex] = vPos.GetY(); 

		return TRUE;
	};
	BOOL   SetAuxPos(int nIndex, double dPosX, double dPosY) 
	{ 
		if (nIndex < 0 || nIndex > m_vdAuxiliaryPosX.size() || nIndex > m_vdAuxiliaryPosY.size())
		{
			return FALSE;
		}

		m_vdAuxiliaryPosX[nIndex] = dPosX; 
		m_vdAuxiliaryPosY[nIndex] = dPosY; 

		return TRUE;
	};

	BOOL   SetAuxPosAngle(int nIndex, sc2Vector vPos, double dAngle)
	{
		if (!SetAuxPos(nIndex, vPos))
		{
			return FALSE;
		}

		m_vdAuxiliaryAngle[nIndex] = dAngle; 

		return TRUE;
	};

	BOOL   SetAuxPosAngle(int nIndex, double dPosX, double dPosY, double dAngle)
	{
		if (!SetAuxPos(nIndex, dPosX, dPosY))
		{
			return FALSE;
		}

		m_vdAuxiliaryAngle[nIndex] = dAngle; 

		return TRUE;
	};

	// *this = *this + vOffset
	void   Offset(sc2Vector vOffset)
	{
		m_dPosX += vOffset.GetX();
		m_dPosY += vOffset.GetY();

		for (int i = 0; i < m_vdAuxiliaryPosX.size(); i++)
		{
			m_vdAuxiliaryPosX[i] += vOffset.GetX();
			m_vdAuxiliaryPosY[i] += vOffset.GetY();
		}
	}

	// *this = *this + (dOffsetX, dOffsetY)
	void   Offset(double dOffsetX, double dOffsetY)
	{
		m_dPosX += dOffsetX;
		m_dPosY += dOffsetY;

		for (int i = 0; i < m_vdAuxiliaryPosX.size(); i++)
		{
			m_vdAuxiliaryPosX[i] += dOffsetX;
			m_vdAuxiliaryPosY[i] += dOffsetY;
		}
	}

	// *other = c * (*this) + t
	CCoordPos Map(sc2Matrix c, sc2Vector t = sc2Vector(0,0))
	{
		sc2XformLinear xform(c, t);				
		CCoordPos cp;
		
		cp = *this;	

		cp.SetPos(xform.MapPoint(GetPos()));

		for (int i = 0; i < m_vdAuxiliaryPosX.size(); i++)
		{
			cp.SetAuxPos(i, xform.MapPoint(GetAuxPos(i)));
		}

		return cp;
	}

	// *other = Inverse(c) * (*this) + Inverse(c)*(-t)
	CCoordPos InvMap(sc2Matrix c, sc2Vector t = sc2Vector(0,0))
	{
		sc2XformLinear xform(c, t);				
		CCoordPos cp;

		cp = *this;	

		cp.SetPos(xform.InvMapPoint(GetPos()));

		for (int i = 0; i < m_vdAuxiliaryPosX.size(); i++)
		{
			cp.SetAuxPos(i, xform.InvMapPoint(GetAuxPos(i)));
		}

		return cp;
	}

	// 用于输出打印类的成员变量；
	void ToString(CString &Str)
	{
		Str = _T("");
		CString StrTmp = _T("");
		StrTmp.Format(_T("PosX:%.3f,"),m_dPosX);
		Str+= StrTmp;
		StrTmp.Format(_T("PosY:%.3f,"),m_dPosY);
		Str+= StrTmp;
		StrTmp.Format(_T("PosD:%.3f,"),m_dAngle);
		Str+= StrTmp;
		for (int it = 0;it<MaxAuxiliaryPosNum;it++)
		{
			StrTmp.Format(_T("AuxiliaryPos%d:PosX:%.3f,PosY:%.3f,PosD:%.3f,"),it,m_vdAuxiliaryPosX.at(it),m_vdAuxiliaryPosY.at(it),m_vdAuxiliaryAngle.at(it));
			Str+= StrTmp;
		}
		Str+= _T("\n");
	}

public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
// 	double m_dWidth;		// 矩形宽度
// 	double m_dHeight;		// 矩形高度
	std::vector<double> m_vdAuxiliaryPosX;	// 辅助位置X
	std::vector<double> m_vdAuxiliaryPosY;	// 辅助位置Y
	std::vector<double> m_vdAuxiliaryAngle;	// 辅助旋转角度，单位度

	double m_dLineAngle1;
	double m_dLineAngle2;
    bool   m_bFindLine1;
    bool   m_bFindLine2;

    scAffineRect m_SearchRect;
    bool   m_bFindSearchRect;

};



// 相机平台类型
typedef enum enumCamPlatformType
{
	eCamPlatformUnknown = -1,				// 未知
	eCamPlatformSepFix  = 0,				// 独立固定安装，只针对XYD、UVW、DXY平台
	eCamPlatformSepX	= 1,				// 独立X轴，只针对XYD、UVW、DXY平台
	eCamPlatformSepXY	= 2,				// 独立XY轴，只针对XYD、UVW、DXY平台
	eCamPlatformShareX						// 平台X轴，只针对YX1X2+D、YD+X1X2平台
} CamPlatformType;


// 拾取相机平台类型
typedef enum enumPickCamPlatformType
{
	ePickCamPlatformUnknown = -1,				// 未知
	ePickCamPlatformSepFix  = 0,				// 独立固定安装，只针对XYD、UVW、DXY平台
	ePickCamPlatformShareXY = 1,				// 共享运动平台的XY，即相机随着拾取头和运动平台一起XY方向运动
	ePickCamPlatformSepX	= 2,				// 独立X轴，只针对XYD、UVW、DXY平台
	ePickCamPlatformSepY	= 3,				// 独立Y轴，只针对XYD、UVW、DXY平台
	ePickCamPlatformSepXY	= 4,				// 独立XY轴，只针对XYD、UVW、DXY平台
} PickCamPlatformType;



// 对位平台类型
typedef enum enumPlatformType
{
	ePlatformUnknown	= -1,					// 未知
	ePlatformXYD		= 0,					// XYD
	ePlatformX1X2Y		= 1,
	ePlatformXY1Y2		= 2,
	ePlatformXY			= 3,					// XY
	ePlatformYX1X2PD	= 4,					// YX+D
	ePlatformYDPX1X2	= 5,					// YD+X
	ePlatformDXY		= 6,					// DXY
	ePlatformXD			= 7,					// XD
	ePlatformYD			= 8,					// YD
	ePlatformXDPY		= 9,					// XD + Y Y方向标定时使用相机轴移动，适用于三力设备
	ePlatformXDPY1		= 10,					// XD + Y Y方向标定时，在Y平台进行吸放动作，适用于风华设备
	ePlatformXYPD		= 11,					// XY + D
	ePlatformX			= 12,					// 只有X平台，四维焊接项目特殊使用
	ePlatformY			= 13,					// Y平台
} PlatformType;


// 坐标轴的方向
// 感觉上是应该与计算时规定的假象坐标系(应该是左手坐标系，即X向右、Y向下、顺时针为正)进行比较，
//	如果反正则为负，如果同向则为正。 因此：
// 对于X、Y、D，则
// X轴是左为负，还是右为正
// Y轴是上为负，还是下为正
// D轴是顺时针为正，还是逆时针为负



typedef enum enumPlatformAxisDirectType
{
	eDirectNegative		= -1,			//  反向，为负
	eDirectPositive		= 1,				//	同向，为正
	eDirectNone		= 2,                  //不动
}PlatformAxisDirectType;

typedef enum enumPlatformUVWSliderType
{
	eFixedSlider		= 0,			//  固定滑块，类型1，转动辅链接在下面的轴上
	eShiftSlider		= 1				//	移动滑块，类型2，转动辅链接找上面的平台上
}PlatformUVWSliderType; 
	
// D轴的驱动类型
typedef enum enumPlatformDDriveType
{
	eDriveDirect     = 0,				//	直接驱动
	eDriveLine		 = 1				//  直线驱动
} PlatformDDriveType;

// D轴的直线驱动的类型
typedef enum enumPlatformDDriveLineType
{
	eDriveLineFixed     = 0,				//	固定，接触点一直在直线轴上
	eDriveLineShift  	  = 1				//  滑动，接触点不一直在直线轴上
} PlatformDDriveLineType;

// 平台实际各轴的位置
class VS_ALIGN_API CPlatformAxisPos
{
public:
	CPlatformAxisPos(){}
	virtual ~CPlatformAxisPos(){}

public:	
	virtual PlatformType GetPlatformType() const  = 0;
};


// XY平台实际各轴的位置
class VS_ALIGN_API CPlatformXYAxisPos : public CPlatformAxisPos
{
public:
	CPlatformXYAxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
	}
	virtual ~CPlatformXYAxisPos(){}
	BOOL operator==(const CPlatformXYAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY);
	}
	BOOL operator!=(const CPlatformXYAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXY;};

public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
};

// X平台实际各轴的位置
class VS_ALIGN_API CPlatformXAxisPos : public CPlatformAxisPos
{
public:
	CPlatformXAxisPos()
	{
		m_dPosX = 0;			// 位置X
	}

	virtual ~CPlatformXAxisPos(){}

	BOOL operator==(const CPlatformXAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX);
	}

	BOOL operator!=(const CPlatformXAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformX;};

public:
	double m_dPosX;			// 位置X
};


// Y平台实际各轴的位置
class VS_ALIGN_API CPlatformYAxisPos : public CPlatformAxisPos
{
public:
	CPlatformYAxisPos()
	{
		m_dPosY = 0;			// 位置Y
	}

	virtual ~CPlatformYAxisPos(){}

	BOOL operator==(const CPlatformYAxisPos& pos)
	{
		return (m_dPosY == pos.m_dPosY);
	}

	BOOL operator!=(const CPlatformYAxisPos& pos)
	{
		return (m_dPosY != pos.m_dPosY);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformY;};

public:
	double m_dPosY;			// 位置Y
};


// XYθ平台实际各轴的位置
class VS_ALIGN_API CPlatformXYDAxisPos : public CPlatformAxisPos
{
public:
	CPlatformXYDAxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
	}
	virtual ~CPlatformXYDAxisPos(){}

	BOOL operator==(const CPlatformXYDAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformXYDAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXYD;};
	
public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
};
// XY+θ平台实际各轴的位置
class VS_ALIGN_API CPlatformXYPDAxisPos : public CPlatformAxisPos
{
public:
	CPlatformXYPDAxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
	}
	virtual ~CPlatformXYPDAxisPos(){}

	BOOL operator==(const CPlatformXYPDAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformXYPDAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXYPD;};
	
public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
};
// YX1X2+θ平台实际各轴的位置
class VS_ALIGN_API CPlatformYX1X2PDAxisPos : public CPlatformAxisPos
{
public:
	CPlatformYX1X2PDAxisPos()
	{
		m_dPosX[0] = 0;			// 位置X0
		m_dPosX[1] = 0;			// 位置X1
		m_dPosY	   = 0;			// 位置Y
		m_dAngle   = 0;			// 旋转角度，单位度
	}
	CPlatformYX1X2PDAxisPos(const CPlatformYX1X2PDAxisPos& pos)
	{
		m_dPosX[0] = pos.m_dPosX[0];
		m_dPosX[1] = pos.m_dPosX[1];
		m_dPosY	   = pos.m_dPosY;
		m_dAngle   = pos.m_dAngle;
	}
	CPlatformYX1X2PDAxisPos& operator= (const CPlatformYX1X2PDAxisPos& pos)
	{
		if (this == &pos)
		{
			return *this;
		}
		m_dPosX[0] = pos.m_dPosX[0];
		m_dPosX[1] = pos.m_dPosX[1];
		m_dPosY	   = pos.m_dPosY;
		m_dAngle   = pos.m_dAngle;
		return *this;
	}
	BOOL operator== (const CPlatformYX1X2PDAxisPos& pos) const
	{
		return (m_dPosX[0] == pos.m_dPosX[0]) && (m_dPosX[1] == pos.m_dPosX[1]) \
			&& (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformYX1X2PDAxisPos& pos) const
	{
		return (m_dPosX[0] != pos.m_dPosX[0]) || (m_dPosX[1] != pos.m_dPosX[1]) \
			|| (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}
	
	virtual ~CPlatformYX1X2PDAxisPos(){}
public:	
	virtual PlatformType GetPlatformType() const {return ePlatformYX1X2PD;};
	
public:
	double m_dPosX[2];			// 位置X
	double m_dPosY;				// 位置Y
	double m_dAngle;			// 旋转角度，单位度
};




// Yθ+X1X2平台实际各轴的位置
class VS_ALIGN_API CPlatformYDPX1X2AxisPos : public CPlatformAxisPos
{
public:
	CPlatformYDPX1X2AxisPos()
	{
		m_dPosX[0] = 0;			// 位置X0
		m_dPosX[1] = 0;			// 位置X1
		m_dPosY	   = 0;			// 位置Y
		m_dAngle   = 0;			// 旋转角度，单位度
	}
	CPlatformYDPX1X2AxisPos(const CPlatformYDPX1X2AxisPos& pos)
	{
		m_dPosX[0] = pos.m_dPosX[0];
		m_dPosX[1] = pos.m_dPosX[1];
		m_dPosY	   = pos.m_dPosY;
		m_dAngle   = pos.m_dAngle;
	}
	CPlatformYDPX1X2AxisPos& operator= (const CPlatformYDPX1X2AxisPos& pos)
	{
		if (this == &pos)
		{
			return *this;
		}
		m_dPosX[0] = pos.m_dPosX[0];
		m_dPosX[1] = pos.m_dPosX[1];
		m_dPosY	   = pos.m_dPosY;
		m_dAngle   = pos.m_dAngle;
		return *this;
	}
	BOOL operator==(const CPlatformYDPX1X2AxisPos& pos) const
	{
		return (m_dPosX[0] == pos.m_dPosX[0]) && (m_dPosX[1] == pos.m_dPosX[1]) \
			&& (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformYDPX1X2AxisPos& pos) const
	{
		return (m_dPosX[0] != pos.m_dPosX[0]) || (m_dPosX[1] != pos.m_dPosX[1]) \
			|| (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}

	virtual ~CPlatformYDPX1X2AxisPos(){}
public:	
	virtual PlatformType GetPlatformType() const {return ePlatformYDPX1X2;};
	
public:
	double m_dPosX[2];			// 位置X
	double m_dPosY;				// 位置Y
	double m_dAngle;			// 旋转角度，单位度
};

// X1X2Y平台实际各轴的位置
class VS_ALIGN_API CPlatformX1X2YAxisPos : public CPlatformAxisPos
{
public:
	CPlatformX1X2YAxisPos()
	{
		m_dPosX1 = 0;			// 位置X1
		m_dPosX2 = 0;			// 位置X2
		m_dPosY = 0;			// 位置Y
	}
	
	virtual ~CPlatformX1X2YAxisPos(){}
	BOOL operator==(const CPlatformX1X2YAxisPos& pos)
	{
		return (m_dPosX1 == pos.m_dPosX1) && (m_dPosX2 == pos.m_dPosX2) && (m_dPosY == pos.m_dPosY);
	}
	BOOL operator!=(const CPlatformX1X2YAxisPos& pos)
	{
		return (m_dPosX1 != pos.m_dPosX1) || (m_dPosX2 != pos.m_dPosX2) || (m_dPosY != pos.m_dPosY);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformX1X2Y;};
	
public:
	double m_dPosX1;			// 位置X1
	double m_dPosX2;			// 位置X2
	double m_dPosY;				// 位置Y
};


// XY1Y2平台实际各轴的位置
class VS_ALIGN_API CPlatformXY1Y2AxisPos : public CPlatformAxisPos
{
public:
	CPlatformXY1Y2AxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY1 = 0;			// 位置Y1
		m_dPosY2 = 0;			// 位置Y2
	}
	
	virtual ~CPlatformXY1Y2AxisPos(){}
	BOOL operator==(const CPlatformXY1Y2AxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY1 == pos.m_dPosY1) && (m_dPosY2 == pos.m_dPosY2);
	}
	BOOL operator!=(const CPlatformXY1Y2AxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY1 != pos.m_dPosY1) || (m_dPosY2 != pos.m_dPosY2);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXY1Y2;};
	
public:
	double m_dPosY1;			// 位置Y1
	double m_dPosY2;			// 位置Y2
	double m_dPosX;			// 位置X
};


// θXY平台实际各轴的位置
class VS_ALIGN_API CPlatformDXYAxisPos : public CPlatformAxisPos
{
public:
	CPlatformDXYAxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
	}
	virtual ~CPlatformDXYAxisPos(){}
	BOOL operator==(const CPlatformDXYAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformDXYAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}
	
public:	
	virtual PlatformType GetPlatformType() const {return ePlatformDXY;};
	
public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
};

// Xθ+Y平台实际各轴的位置
class VS_ALIGN_API CPlatformXDPYAxisPos : public CPlatformAxisPos
{
public:
	CPlatformXDPYAxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
	}
	virtual ~CPlatformXDPYAxisPos(){}
	BOOL operator==(const CPlatformXDPYAxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformXDPYAxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXDPY;};

public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
};

// Xθ+Y1平台实际各轴的位置
class VS_ALIGN_API CPlatformXDPY1AxisPos : public CPlatformAxisPos
{
public:
	CPlatformXDPY1AxisPos()
	{
		m_dPosX = 0;			// 位置X
		m_dPosY = 0;			// 位置Y
		m_dAngle = 0;			// 旋转角度，单位度
	}
	virtual ~CPlatformXDPY1AxisPos(){}
	BOOL operator==(const CPlatformXDPY1AxisPos& pos)
	{
		return (m_dPosX == pos.m_dPosX) && (m_dPosY == pos.m_dPosY) && (m_dAngle == pos.m_dAngle);
	}
	BOOL operator!=(const CPlatformXDPY1AxisPos& pos)
	{
		return (m_dPosX != pos.m_dPosX) || (m_dPosY != pos.m_dPosY) || (m_dAngle != pos.m_dAngle);
	}

public:	
	virtual PlatformType GetPlatformType() const {return ePlatformXDPY1;};

public:
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
};

// 平台类型信息
class VS_ALIGN_API CPlatformInfo
{
public:
	CPlatformInfo(){};
	virtual ~CPlatformInfo(){};

public:
	virtual PlatformType GetPlatformType() const = 0;		// 获取平台类型
	
};


// XY平台类型信息
class VS_ALIGN_API CPlatformXYInfo : public CPlatformInfo
{
	
public:
	CPlatformXYInfo()
	{
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// // 平台相机Y轴坐标方向：向下为正，向上为负
	}
	
	virtual ~CPlatformXYInfo(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformXY;}
	
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;		
	PlatformAxisDirectType		m_nPlatformYCoordType;		
};

// X平台类型信息
class VS_ALIGN_API CPlatformXInfo : public CPlatformInfo
{

public:
	CPlatformXInfo()
	{
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// // 平台相机Y轴坐标方向：向下为正，向上为负
	}

	virtual ~CPlatformXInfo(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformX;}

public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;		
	PlatformAxisDirectType		m_nPlatformYCoordType;		
};


// Y平台类型信息
class VS_ALIGN_API CPlatformYInfo : public CPlatformInfo
{

public:
	CPlatformYInfo()
	{
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;
		m_nPlatformYCoordType = eDirectNegative;
	}

	virtual ~CPlatformYInfo(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformY;}

public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;		
	PlatformAxisDirectType		m_nPlatformYCoordType;		
};


// XYθ平台类型信息
class VS_ALIGN_API CPlatformXYDInfo : public CPlatformInfo
{

public:
	CPlatformXYDInfo()
	{	
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负
		
		// XYD平台的详细信息
		m_nPlatformDDriveType = eDriveDirect;					// DD马达驱动方式：直接驱动，直线驱动
		m_nPlatformDDriveLineType = eDriveLineFixed;		// DD马达直线驱动方式:0为固定，1位滑动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm

	}
	
	virtual ~CPlatformXYDInfo(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformXYD;}
 
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;			// 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;		// 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;		// 旋转方向：顺时针为正，逆时针方向为负

	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;			// DD马达驱动方式：0为直接驱动，1为直线驱动
	PlatformDDriveLineType	m_nPlatformDDriveLineType; // DD马达直线驱动方式:0为固定，1位滑动
	double	m_dPlatformRotationLength;						// 平台旋转中心至电机轴中心的距离

	// 是否可以根据运动量计算旋转角度?
	
};

// XY+θ平台类型信息
class VS_ALIGN_API CPlatformXYPDInfo : public CPlatformInfo
{

public:
	CPlatformXYPDInfo()
	{	
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负

		// XYPD平台的详细信息
		m_nPlatformDDriveType = eDriveDirect;					// DD马达驱动方式：直接驱动，直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm

	}

	virtual ~CPlatformXYPDInfo(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformXYPD;}

public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;			// 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;		// 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;		// 旋转方向：顺时针为正，逆时针方向为负

	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;			// DD马达驱动方式：0为直接驱动，1为直线驱动
	double	m_dPlatformRotationLength;						// 平台旋转中心至电机轴中心的距离

	// 是否可以根据运动量计算旋转角度?

};
// Xθ+Y平台类型信息
class VS_ALIGN_API CPlatformXDPYInfo : public CPlatformInfo
{

public:
	CPlatformXDPYInfo()
	{	
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负

		// XYD平台的详细信息
		m_nPlatformDDriveType = eDriveDirect;					// DD马达驱动方式：直接驱动，直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm


	}

	virtual ~CPlatformXDPYInfo(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformXDPY;}

public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;			// 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;		// 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;		// 旋转方向：顺时针为正，逆时针方向为负

	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;			// DD马达驱动方式：0为直接驱动，1为直线驱动
	double	m_dPlatformRotationLength;						// 平台旋转中心至电机轴中心的距离


};

// Xθ+Y1平台类型信息
class VS_ALIGN_API CPlatformXDPY1Info : public CPlatformInfo
{

public:
	CPlatformXDPY1Info()
	{	
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负

		// XYD平台的详细信息
		m_nPlatformDDriveType = eDriveDirect;					// DD马达驱动方式：直接驱动，直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm


	}

	virtual ~CPlatformXDPY1Info(){}

public:
	virtual PlatformType GetPlatformType() const {return ePlatformXDPY1;}

public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;			// 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;		// 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;		// 旋转方向：顺时针为正，逆时针方向为负

	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;			// DD马达驱动方式：0为直接驱动，1为直线驱动
	double	m_dPlatformRotationLength;						// 平台旋转中心至电机轴中心的距离


};

// YX1X2+θ平台类型信息
class VS_ALIGN_API CPlatformYX1X2PDInfo  : public CPlatformInfo
{

public:
	CPlatformYX1X2PDInfo()
	{
		// 平台信息		
		m_nPlatformXCoordType[0] = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformXCoordType[1] = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负

		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负
		
		// XYD平台的详细信息
		m_nPlatformDDriveType = eDriveLine;					// DD马达驱动方式：0为直接驱动，1为直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm
		
	}
	
	virtual ~CPlatformYX1X2PDInfo(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformYX1X2PD;}
	
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType[2];		// // 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;			// // 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;			// 旋转方向：顺时针为正，逆时针方向为负
	
	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;				// DD马达驱动方式：0为直接驱动，1为直线驱动
	double	m_dPlatformRotationLength;							// 平台旋转中心至电机轴中心的距离
	
};

// Yθ+X1X2平台类型信息
class VS_ALIGN_API CPlatformYDPX1X2Info  : public CPlatformInfo
{
public:
	CPlatformYDPX1X2Info()
	{
		// 平台信息		
		m_nPlatformXCoordType[0] = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		m_nPlatformXCoordType[1] = eDirectPositive;			// 平台相机X轴坐标方向：向右为正，向左为负
		
		m_nPlatformYCoordType = eDirectNegative;			// 平台相机Y轴坐标方向：向下为正，向上为负
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：顺时针为正，逆时针方向为负
		
		// XYD平台的详细信息
		m_nPlatformDDriveType = eDriveLine;					// DD马达驱动方式：0为直接驱动，1为直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm
		
	}
	
	virtual ~CPlatformYDPX1X2Info(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformYDPX1X2;}
	
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType[2];		// 平台相机X轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;			// 平台相机Y轴坐标方向：向下为正，向上为负
	PlatformAxisDirectType		m_nPlatformDCoordType;			// 旋转方向：顺时针为正，逆时针方向为负
	
	// XYD平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;				
	double	m_dPlatformRotationLength;							
	
};



// X1X2Y平台类型信息
class VS_ALIGN_API CPlatformX1X2YInfo : public CPlatformInfo
{
	
public:
	CPlatformX1X2YInfo()
	{
		// 平台信息		
		m_nPlatformX1CoordType = eDirectPositive;			// X1轴方向，向右为正，向左为负
		m_nPlatformX2CoordType = eDirectPositive;			// X2轴方向：向右为正，向左为负
		m_nPlatformYCoordType = eDirectPositive;			// Y轴方向：向上为正，向下为负
//		m_bInited = FALSE;
//		m_iUVWPltfmType = -1;

		m_X1Pos.m_dPosX = 67.5;
		m_X1Pos.m_dPosY = -57;

		m_X2Pos.m_dPosX = 67.5;
		m_X2Pos.m_dPosY = 57;

		m_YPos.m_dPosX = -57;
		m_YPos.m_dPosY = 67.5;

		m_nPlatformUVWSliderType = eFixedSlider;

	}
	
	virtual ~CPlatformX1X2YInfo(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformX1X2Y;}
	
	void	SetX1X2YPos(CCoordPos X1pos,CCoordPos X2pos,CCoordPos Ypos)
	{
		m_X1Pos=X1pos; m_X2Pos=X2pos; m_YPos= Ypos;
	}
	
	void	GetX1X2YPos(CCoordPos& X1pos, CCoordPos& X2pos, CCoordPos& Ypos)
	{X1pos = m_X1Pos; X2pos = m_X2Pos; Ypos = m_YPos;	}

	CCoordPos  GetX1Pos(){return m_X1Pos;}
	CCoordPos  GetX2Pos(){return m_X2Pos;}
	CCoordPos  GetYPos(){return m_YPos;}


	//初始化，判断UVW平台的方向问题
//	BOOL Init();
	void ConvertX1X2YPltfmPosToXYDPltfmPos(double dX1, double dX2, double dY, double& dX0, double& dY0, double& dD0);
	void ConvertXYDPltfmPosToX1X2YPltfmPos(double dX0, double dY0, double dD0, double& dX1, double& dX2, double& dY);
	
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformX1CoordType;		// X1轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformX2CoordType;		// X2轴坐标方向：向右为正，向左为负
	PlatformAxisDirectType		m_nPlatformYCoordType;		// Y轴坐标方向：向上为正，向下为负
	
	CCoordPos  m_X1Pos;				// X1X2Y平台共享参数：X1X2Y坐标位置
	CCoordPos  m_X2Pos;
	CCoordPos  m_YPos;

	PlatformUVWSliderType		m_nPlatformUVWSliderType;	// UVW平台安装结构类型:类型1、类型2？
	
};




// XY1Y2平台类型信息
class VS_ALIGN_API CPlatformXY1Y2Info : public CPlatformInfo
{
	
public:
	CPlatformXY1Y2Info()
	{
		// 平台信息		
		m_nPlatformY1CoordType = eDirectPositive;			// Y1轴方向，向上为正，向下为负
		m_nPlatformY2CoordType = eDirectPositive;			// Y2轴方向：向上为正，向下为负
		m_nPlatformXCoordType = eDirectPositive;			// X轴方向：向右为正，向左为负

// 		m_Y1Pos.m_dPosX = -57;
// 		m_Y1Pos.m_dPosY = -67.5;
// 		
// 		m_Y2Pos.m_dPosX = 57;
// 		m_Y2Pos.m_dPosY = -67.5;
// 		
// 		m_XPos.m_dPosX = 67.5;
// 		m_XPos.m_dPosY = 57;
		
		
		m_Y1Pos.m_dPosX = -78.5;
		m_Y1Pos.m_dPosY = 79;

		m_Y2Pos.m_dPosX = 78.5;
		m_Y2Pos.m_dPosY = 79;

		m_XPos.m_dPosX = 70;
		m_XPos.m_dPosY = -78.5;

		m_nPlatformUVWSliderType = eFixedSlider;

	}
	
	virtual ~CPlatformXY1Y2Info(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformXY1Y2;}
	
	void	SetXY1Y2Pos(CCoordPos Y1Pos, CCoordPos Y2Pos, CCoordPos XPos)
	{m_Y1Pos=Y1Pos; m_Y2Pos=Y2Pos; m_XPos= XPos;}

	
	void	GetXY1Y2Pos(CCoordPos& Y1Pos, CCoordPos& Y2Pos, CCoordPos& XPos)
	{
		Y1Pos = m_Y1Pos; Y2Pos = m_Y2Pos; XPos = m_XPos;
	}
	CCoordPos  GetY1Pos(){return m_Y1Pos;}
	CCoordPos  GetY2Pos(){return m_Y2Pos;}
	CCoordPos  GetXPos(){return m_XPos;}

	void ConvertXY1Y2PltfmPosToXYDPltfmPos(double dY1, double dY2, double dX, double& dX0, double& dY0, double& dD0);
	void ConvertXYDPltfmPosToXY1Y2PltfmPos(double dX0, double dY0, double dD0, double& dY1, double& dY2, double& dX);


public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformY1CoordType;		// Y1轴坐标方向：向上为正，向下为负
	PlatformAxisDirectType		m_nPlatformY2CoordType;		// Y2轴坐标方向：向上为正，向下为负
	PlatformAxisDirectType		m_nPlatformXCoordType;		// X轴坐标方向：向左为正，向右为负
	
	CCoordPos  m_Y1Pos;				// XY1Y2平台共享参数：XY1Y2各点位置(>0)
	CCoordPos  m_Y2Pos;
	CCoordPos  m_XPos;

	PlatformUVWSliderType		m_nPlatformUVWSliderType;	// UVW平台安装结构类型:类型1、类型2？

	
};


// θXY平台类型信息
class VS_ALIGN_API CPlatformDXYInfo : public CPlatformInfo
{
	
public:
	CPlatformDXYInfo()
	{
		// 平台信息		
		m_nPlatformXCoordType = eDirectPositive;			// 平台左相机CH0的X轴坐标方向：0为向左，1为向右
		m_nPlatformYCoordType = eDirectNegative;			// 平台坐标系类型：0为右手系，1为左手系(即平台Y轴坐标方向：0为向上，1为向下)
		m_nPlatformDCoordType = eDirectNegative;			// 旋转方向：0为顺时针方向，1为逆时针方向
		
		// DXY平台的详细信息
		m_nPlatformDDriveType = eDriveLine;					// DD马达驱动方式：0为直接驱动，1为直线驱动
		m_dPlatformRotationLength = 100.0;					// 平台旋转中心至电机轴中心的距离，单位为mm
		
	}
	
	virtual ~CPlatformDXYInfo(){}
	
public:
	virtual PlatformType GetPlatformType() const {return ePlatformDXY;}
	
public:
	// 平台信息
	PlatformAxisDirectType		m_nPlatformXCoordType;		// 平台左相机CH0的X轴坐标方向：0为向左，1为向右
	PlatformAxisDirectType		m_nPlatformYCoordType;		// 平台坐标系类型：0为右手系，1为左手系(即平台Y轴坐标方向：0为向上，1为向下)
	PlatformAxisDirectType		m_nPlatformDCoordType;		// 旋转方向：0为顺时针方向，1为逆时针方向
	
	// DXY平台的详细信息
	PlatformDDriveType		m_nPlatformDDriveType;			// DD马达驱动方式：0为直接驱动，1为直线驱动
	double	m_dPlatformRotationLength;						// 平台旋转中心至电机轴中心的距离
	
	// 是否可以根据运动量计算旋转角度?
	
};

// 相机平台类型信息
class VS_ALIGN_API CCamPlatformInfo
{
public:
	CCamPlatformInfo()
	{
		m_nCameraNum = 2;
		m_nCameraFixType = eCamPlatformSepFix;
// 		m_vnCamDirectXs.clear();
// 		m_vnCamDirectYs.clear();
	}
	CCamPlatformInfo(const CCamPlatformInfo& platformInfo)
	{
		m_nCameraNum = platformInfo.m_nCameraNum;
		m_nCameraFixType = platformInfo.m_nCameraFixType;
		m_vnCamDirectXs = platformInfo.m_vnCamDirectXs;
		m_vnCamDirectYs = platformInfo.m_vnCamDirectYs;
		m_vnCamPlatformAxisDirectXs = platformInfo.m_vnCamPlatformAxisDirectXs;
		m_vnCamPlatformAxisDirectYs = platformInfo.m_vnCamPlatformAxisDirectYs;
	}

	virtual ~CCamPlatformInfo(){};

public:
	void SetCamNumPlatformType(int nCamNum, CamPlatformType nType)		// 设置相机个数和相机平台类型
	{
		m_vnCamDirectXs.resize(nCamNum, eDirectPositive);
		m_vnCamDirectYs.resize(nCamNum, eDirectPositive);
		
		m_vnCamPlatformAxisDirectXs.resize(nCamNum,eDirectPositive);
		m_vnCamPlatformAxisDirectYs.resize(nCamNum,eDirectPositive);
		
		m_nCameraNum = nCamNum; 
		m_nCameraFixType = nType;
	}
	
	int GetCamNum() const {return m_nCameraNum;}							// 获取相机个数
	CamPlatformType GetCamPlatformType() const {return m_nCameraFixType; }	// 获取相机平台类型

	BOOL SetCamDirectX(int nIndex, PlatformAxisDirectType nDirectType)       // 设置相机平台X轴类型
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamDirectXs.size()));
		if ((nIndex<0) || (nIndex>=m_vnCamDirectXs.size()))
		{
			return FALSE;
		}
		else
		{
			m_vnCamDirectXs.at(nIndex) = nDirectType;
			return TRUE;
		}
	}

	BOOL SetCamPlatformAxisDirectX(int nIndex, PlatformAxisDirectType nDirectType)       // 设置相机平台所在X轴类型
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamPlatformAxisDirectXs.size()));
		if ((nIndex<0) || (nIndex>=m_vnCamPlatformAxisDirectXs.size()))
		{
			return FALSE;
		}
		else
		{
			m_vnCamPlatformAxisDirectXs.at(nIndex) = nDirectType;
			return TRUE;
		}
	}
	
	BOOL SetCamDirectY(int nIndex, PlatformAxisDirectType nDirectType)		// 设置相机平台Y轴类型
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamDirectYs.size()));
		if ((nIndex<0) || (nIndex>=m_vnCamDirectYs.size()))
		{
			return FALSE;
		}
		else
		{
			m_vnCamDirectYs.at(nIndex) = nDirectType;
			return TRUE;
		}
	}	
	BOOL SetCamPlatformAxisDirectY(int nIndex, PlatformAxisDirectType nDirectType)       // 设置相机平台所在X轴类型
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamPlatformAxisDirectYs.size()));
		if ((nIndex<0) || (nIndex>=m_vnCamPlatformAxisDirectYs.size()))
		{
			return FALSE;
		}
		else
		{
			m_vnCamPlatformAxisDirectYs.at(nIndex) = nDirectType;
			return TRUE;
		}
	}
	
	int		GetCamPlatformNum() const {return m_nCameraNum;}							// 获取相机运动平台的个数
	int		GetCamPlatformXNum() const {return m_vnCamDirectXs.size();}				// 获取相机平台的X轴个数
	int		GetCamPlatformYNum() const {return m_vnCamDirectYs.size();}				// 获取相机平台的Y轴个数

	PlatformAxisDirectType	GetCamDirectX (int nIndex) const	// 获取第nIndex相机平台的X轴方向
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamDirectXs.size()));
		return m_vnCamDirectXs.at(nIndex);
	}

	PlatformAxisDirectType	GetCamDirectY (int nIndex) const	// 获取第nIndex相机平台的Y轴方向
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamDirectYs.size()));
		return m_vnCamDirectYs.at(nIndex);
	}
	
	PlatformAxisDirectType GetCamPlatformAxisDirectX(int nIndex) const
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamPlatformAxisDirectXs.size()));
		return m_vnCamPlatformAxisDirectXs.at(nIndex);
	}
	PlatformAxisDirectType GetCamPlatformAxisDirectY(int nIndex) const
	{
		ASSERT((nIndex>=0) && (nIndex<m_vnCamPlatformAxisDirectYs.size()));
		return m_vnCamPlatformAxisDirectYs.at(nIndex);
	}
private:
	//      相机平台相关信息
	int		m_nCameraNum;														// 相机数量
	CamPlatformType		m_nCameraFixType;										// 相机安装方式：0为固定安装，1为安装在平台X轴上，2为安装在独立的XY轴上，3为安装在单独的X轴上
	
	std::vector<PlatformAxisDirectType>	m_vnCamDirectXs;						// 平台相机X轴坐标方向
	std::vector<PlatformAxisDirectType>	m_vnCamDirectYs;						// 平台相机Y轴坐标方向

	std::vector<PlatformAxisDirectType> m_vnCamPlatformAxisDirectXs;			//相机平台所在轴的X方向
	std::vector<PlatformAxisDirectType> m_vnCamPlatformAxisDirectYs;			//相机平台所在轴的Y方向

};




// 相机工作状态，包含相机是否被选中、使用哪个模板等信息
typedef struct tagPosPatInfo
{
	int		m_nIndex;			// 相机索引号
	BOOL	m_bIsSelected;		// 相机是否被选中
	int		m_nPatNum;			// 相机模板总数
	int		m_nPatSelected;		// 被选中的模板的序号（0-based）
	std::vector<double> m_vdShutter;	// 相机曝光时间，分别记录每个模板的曝光时间

	tagPosPatInfo()
	{
		m_nIndex = -1;
		m_bIsSelected = true;
		m_nPatNum = TARGETCANDIDATENUM;
		m_nPatSelected = 0;
		m_vdShutter.resize(m_nPatNum, 0.01);
	}
} CPosPatInfo;

//20160510增加对角补偿
/************************************************************************************
/* Function name	: AngleOffsetCalculate
/* Description	    : 根据产品尺寸计算对角线角度偏移。
/* Return type		: bool 
/* Argument
/* [in] --  double dObjectL1:	对象水平长度L1
/* [in] --  double dObjectW1:	对象垂直长度W1
/* [in] --  double dTargetL2:	目标水平长度L2
/* [in] --  double dTargetW2:	目标水平长度W2
/* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
/* [in\out] --  scDegree& angleOffset:	返回对角线角度偏移
/************************************************************************************/
bool VS_ALIGN_API AngleOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, int nPosMode, scDegree& angleOffset);

//20160510增加对角补偿
/************************************************************************************
/* Function name	: XYOffsetCalculate
/* Description	    : 根据产品尺寸计算对角线角度偏移。
/* Return type		: bool 
/* Argument
/* [in] --  double dObjectL1:	对象水平长度L1
/* [in] --  double dObjectW1:	对象垂直长度W1
/* [in] --  double dTargetL2:	目标水平长度L2
/* [in] --  double dTargetW2:	目标水平长度W2
/* [in] --  double dF1:			对象目标水平偏移F1
/* [in] --  double dF2:			对象目标水平偏移F2
/* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
/* [in\out] --  double& dOffsetX:	返回X偏移
/* [in\out] --  double& dOffsetY:	返回Y偏移
/************************************************************************************/
bool VS_ALIGN_API XYOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, double dF1, double dF2, int nPosMode, double& dOffsetX, double& dOffsetY);


//20160510增加对角补偿
/************************************************************************************
/* Function name	: AngleDiagonalCalculate
/* Description	    : 根据产品尺寸计算对角线角度。
/* Return type		: bool 
/* Argument
/* [in] --  double dObjectL1:	对象水平长度L1
/* [in] --  double dObjectW1:	对象垂直长度W1
/* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
/* [in\out] --  scDegree& angleDiagonal:	返回对角线角度
/************************************************************************************/
bool VS_ALIGN_API AngleDiagonalCalculate(double dObjectL1, double dObjectW1, int nPosMode, scDegree& angleDiagonal);


