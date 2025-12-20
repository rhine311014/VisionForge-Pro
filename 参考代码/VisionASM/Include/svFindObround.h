#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svCircle.h"
#include "svCircularArc.h"
#include "svAffineRect.h"
#include "svObround.h"
#include "svCaliper.h"

#pragma pack(push,8)

class SV2DMEASURE_API scFindObround
{
public:

	// 自由度
	enum ObroundSeg
	{
		eObroundCircularArcXO		= 1,	// 圆弧1
		eObroundCircularArcYOPP		= 2,	// 圆弧2
		eObroundLineSegOY			= 4,	// 线段1
		eObroundLineSegOPPX			= 8,	// 线段2
		eObroundSegAll		= eObroundCircularArcXO|eObroundCircularArcYOPP|eObroundLineSegOY|eObroundLineSegOPPX, // 所有的自由度
	};

public:
	scFindObround();
	~scFindObround();

	void SetCenterLengthsRotation(const sc2Vector& center,			// 设置中心center，x边长xLen，y边长yLen，旋转角度xRotation
		double xLen, double yLen,
		const scRadian& xRotation = scRadian(0));

	void SetCenterLengthsRotation(double centerX,double centerY, 
		double xLen, double yLen,double xRotation);

	void GetCenterLengthsRotation(sc2Vector& center,					// 获取中心center，x边长xLen，y边长yLen，旋转角度xRotation
		double& xLen, double& yLen, scRadian& xRotation);

	void SetCornerPoLengthsRotation(const sc2Vector& cornerPo,		// 设置原点cornerPo，x边长xLen，y边长yLen，旋转角度xRotation
		double xLen, double yLen,
		const scRadian& xRotation = scRadian(0));

	void SetCornerPoLengthsRotation(double cornerPoX,double cornerPoY, 
		double xLen, double yLen,double xRotation);

	void GetCornerPoLengthsRotation(sc2Vector& cornerPo,				// 获取原点cornerPo，x边长xLen，y边长yLen，旋转角度xRotation
		double& xLen, double& yLen, scRadian& xRotation);

	void	SetObround(const scObround& obround);						// 设置圆弧
	scObround GetObround() const;										// 获取圆弧

	void	SetCaliperParams(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);		// 设置卡尺投影长度
	int		GetCaliperNum()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirection()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			// 获取卡尺投影长度

	// 获取第nIndex个卡尺的参数
	void GetCaliper(ObroundSeg seg,						// 长圆段
					int	   nIndex,						// 卡尺序号，序号范围从0到nCaliperNum-1
					double& dCenterX,					// 仿射矩形（卡尺）中心点的X坐标  
					double& dCenterY,					// 仿射矩形（卡尺）中心点的Y坐标 
					double& dSideXLength,				// 仿射矩形（卡尺）X边长度 
					double& dSideYLength,				// 仿射矩形（卡尺）Y边长度 
					scRadian& rRotation,				// 仿射矩形（卡尺）旋转角度（单位弧度）
					scRadian& rSkew)	const;			// 仿射矩形（卡尺）切变角度（单位弧度）
	scAffineRect GetCaliper(ObroundSeg seg, int nIndex) const;			// 获取第nIndex个卡尺	

	// 设置卡尺属性
	void SetCaliperProperty(double dContrastThreshold,			// 设置卡尺对比度阈值
		EdgePolarity nEdgePolarity,			// 设置卡尺极性
		long lSize);						// 设置卡尺滤波器半宽尺寸
	void SetCaliperProperty(double dContrastThreshold,			// 设置卡尺对比度阈值
		EdgePolarity nEdgePolarity1,		// 设置卡尺边缘1极性
		EdgePolarity nEdgePolarity2,		// 设置卡尺边缘2极性
		long lSize);						// 设置卡尺滤波器半宽尺寸
	void EnableCaliperSortByScore(BOOL bSortByScore);
	void EnableCaliperSortReverse(BOOL bSortReverse);
	
	// 获取卡尺属性
	void GetCaliperProperty(double& dContrastThreshold, EdgePolarity& nEdgePolarity, long& lSize);
	void GetCaliperProperty(double& dContrastThreshold, EdgePolarity& nEdgePolarity1, EdgePolarity& nEdgePolarity2, long& lSize);

	// 设置拟合方法
	void SetFitMethod(FitMethod method);

	// 获取拟合方法
	FitMethod GetFitMethod();
	
	// 设置局外点个数
	bool SetOutLineNum(int num, OutLineMethod method = eRobust);
	bool SetOutLinePrecent(int precent, OutLineMethod method = eRobust);	//precent:0~100

	// 执行FindObround工具
	bool Execute(const scImageGray& image, scObround& obround, double& rms);
	bool Execute(cpImage& image, scObround& obround, double& rms);

	// 获取参与拟合的数据点
	bool GetFitData(ObroundSeg seg, svStd vector<sc2Vector>& points);
	bool GetUsed(ObroundSeg seg, svStd vector<bool>& used);

	// 获取卡尺工具指针
	scCaliperOneEdge* GetCaliperToolOneEdge();
	scCaliperTwoEdge* GetCaliperToolTwoEdge();

private:
	scCaliperOneEdge m_CaliperToolOneEdge;
	scCaliperTwoEdge m_CaliperToolTwoEdge;
	
	void SetDefaults();
	void UpdateCaliper();

	scObround m_Obround;
	scAffineRect* m_pAffineRectCircularArcXO;
	scAffineRect* m_pAffineRectCircularArcYOPP;
	scAffineRect* m_pAffineRectLineSegOY;
	scAffineRect* m_pAffineRectLineSegOPPX;

	int m_nCaliperNum;
	CaliperSearchDirection	m_nSearchDirection;
	double m_dSearchLength;
	double m_dProjectionLength;

	svStd vector<sc2Vector> m_CaplierResultCircularArcXO;
	svStd vector<sc2Vector> m_CaplierResultCircularArcYOPP;
	svStd vector<sc2Vector> m_CaplierResultLineSegOY;
	svStd vector<sc2Vector> m_CaplierResultLineSegOPPX;
	svStd vector<bool> m_bUsedCircularArcXO;
	svStd vector<bool> m_bUsedCircularArcYOPP;
	svStd vector<bool> m_bUsedLineSegOY;
	svStd vector<bool> m_bUsedLineSegOPPX;

	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lSize;
	FitMethod m_Method;

	bool m_bNumOrPrecent;			//true:数量;false:百分比
	long m_nOutLinePrecent;
	long m_nOutLineNum;
	OutLineMethod m_bMethod;

	bool m_bSortByScore;
	bool m_bSortReverse;
};

#pragma pack(pop)