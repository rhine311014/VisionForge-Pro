#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svCircle.h"
#include "svCircularArc.h"
#include "svAffineRect.h"
#include "svGenRect.h"
#include "svCaliper.h"

#pragma pack(push,8)

class SV2DMEASURE_API scFindGenRect
{
public:

	// 自由度
	enum GenRectSeg
	{
		eGenRectLineSegOY		= 1,	// 线段1
		eGenRectLineSegYOPP		= 2,	// 线段2
		eGenRectLineSegOPPX		= 4,	// 线段3
		eGenRectLineSegXO		= 8,	// 线段4
		eGenRectCircularArcPo	= 16,	// 圆弧1
		eGenRectCircularArcPx	= 32,	// 圆弧2
		eGenRectCircularArcPy	= 64,	// 圆弧3
		eGenRectCircularArcPopp	= 128,	// 圆弧4
		eGenRectSegAll	= eGenRectLineSegOY|eGenRectLineSegYOPP|eGenRectLineSegOPPX|eGenRectLineSegXO|eGenRectCircularArcPo|eGenRectCircularArcPx|eGenRectCircularArcPy|eGenRectCircularArcPopp, // 所有的自由度
	};

	// 寻找方法
	enum FindMethod
	{
		eFind4Line			= 1,
		eFind4Circle		= 2,
	};

public:
	scFindGenRect();
	~scFindGenRect();

	void SetCenterLengthsRotationRound(const sc2Vector& center,			// 设置中心center，x边长xLen，y边长yLen，旋转角度xRotation
		double xLen, double yLen,
		const scRadian& xRotation, double dRound);
	
	void SetCenterLengthsRotationRound(double centerX,double centerY, 
		double xLen, double yLen,
		double xRotation,double dRound);

	void GetCenterLengthsRotationRound(sc2Vector& center,					// 获取中心center，x边长xLen，y边长yLen，旋转角度xRotation
		double& xLen, double& yLen, scRadian& xRotation, double& dRound);

	void SetCornerPoLengthsRotationRound(const sc2Vector& cornerPo,		// 设置原点cornerPo，x边长xLen，y边长yLen，旋转角度xRotation
		double xLen, double yLen,
		const scRadian& xRotation, double dRound);

	void SetCornerPoLengthsRotationRound(double cornerPoX,double cornerPoY, 
		double xLen, double yLen,
		double xRotation,double dRound);

	void GetCornerPoLengthsRotationRound(sc2Vector& cornerPo,				// 获取原点cornerPo，x边长xLen，y边长yLen，旋转角度xRotation
		double& xLen, double& yLen, scRadian& xRotation, double& dRound);

	void	SetGenRect(const scGenRect& genRect);						// 设置圆弧
	scGenRect GetGenRect() const;										// 获取圆弧


	void	SetCaliperParams(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);		// 设置卡尺投影长度



	void	SetCaliperParamsLineX(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNumLineX(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirectionLineX(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLengthLineX(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLengthLineX(double dProjectionLength);		// 设置卡尺投影长度
	int		GetCaliperNumLineX()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirectionLineX()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLengthLineX()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthLineX()	const;			// 获取卡尺投影长度


	void	SetCaliperParamsLineY(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNumLineY(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirectionLineY(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLengthLineY(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLengthLineY(double dProjectionLength);		// 设置卡尺投影长度
	int		GetCaliperNumLineY()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirectionLineY()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLengthLineY()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthLineY()	const;			// 获取卡尺投影长度


	void	SetCaliperParamsCircle(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNumCircle(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirectionCircle(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLengthCircle(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLengthCircle(double dProjectionLength);		// 设置卡尺投影长度
	int		GetCaliperNumCircle()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirectionCircle()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLengthCircle()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthCircle()	const;			// 获取卡尺投影长度


	// 获取第nIndex个卡尺的参数
	void GetCaliper(GenRectSeg seg,						// 段
					int	   nIndex,						// 卡尺序号，序号范围从0到nCaliperNum-1
					double& dCenterX,					// 仿射矩形（卡尺）中心点的X坐标  
					double& dCenterY,					// 仿射矩形（卡尺）中心点的Y坐标 
					double& dSideXLength,				// 仿射矩形（卡尺）X边长度 
					double& dSideYLength,				// 仿射矩形（卡尺）Y边长度 
					scRadian& rRotation,				// 仿射矩形（卡尺）旋转角度（单位弧度）
					scRadian& rSkew)	const;			// 仿射矩形（卡尺）切变角度（单位弧度）
	scAffineRect GetCaliper(GenRectSeg seg, int nIndex) const;			// 获取第nIndex个卡尺	

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
	bool Execute(const scImageGray& image, scGenRect& genRect, double& rms, FindMethod method = eFind4Line);
	bool Execute(cpImage& image, scGenRect& genRect, double& rms, FindMethod method = eFind4Line);

	// 获取参与拟合的数据点
	bool GetFitData(GenRectSeg seg, svStd vector<sc2Vector>& points);
	bool GetUsed(GenRectSeg seg, svStd vector<bool>& used);

	// 获取卡尺工具指针
	scCaliperOneEdge* GetCaliperToolOneEdge();
	scCaliperTwoEdge* GetCaliperToolTwoEdge();

	// 获取找边结果
	bool GetLineOY(scLine& lineOY);
	bool GetLineYOPP(scLine& lineYOPP);
	bool GetLineOPPX(scLine& lineOPPX);
	bool GetLineXO(scLine& lineXO);

	// 获取找圆结果
	bool GetCirclePo(scCircle& circlePo);
	bool GetCirclePx(scCircle& circlePx);
	bool GetCirclePy(scCircle& circlePy);
	bool GetCirclePopp(scCircle& circlePopp);

private:
	scCaliperOneEdge m_CaliperToolOneEdge;
	scCaliperTwoEdge m_CaliperToolTwoEdge;
	
	void SetDefaults();
	void UpdateCaliper();
	void UpdateCaliperLineX();
	void UpdateCaliperLineY();
	void UpdateCaliperCircle();

	bool FindLineX(const scImageGray& image);
	bool FindLineY(const scImageGray& image);
	bool FindCircle(const scImageGray& image);

	scLine	m_ResultLineOY;
	scLine	m_ResultLineYOPP;
	scLine	m_ResultLineOPPX;
	scLine	m_ResultLineXO;
	scCircle m_ResultCirclePo;
	scCircle m_ResultCirclePx;
	scCircle m_ResultCirclePy;
	scCircle m_ResultCirclePopp;

	double m_dRmsLineOY;
	double m_dRmsLineYOPP;
	double m_dRmsLineOPPX;
	double m_dRmsLineXO;
	double m_dRmsCirclePo;
	double m_dRmsCirclePx;
	double m_dRmsCirclePy;
	double m_dRmsCirclePopp;

	bool m_bSuccessLineOY;
	bool m_bSuccessLineYOPP;
	bool m_bSuccessLineOPPX;
	bool m_bSuccessLineXO;
	bool m_bSuccessCirclePo;
	bool m_bSuccessCirclePx;
	bool m_bSuccessCirclePy;
	bool m_bSuccessCirclePopp;

	scGenRect m_GenRect;
	scAffineRect* m_pAffineRectLineSegOY;
	scAffineRect* m_pAffineRectLineSegYOPP;
	scAffineRect* m_pAffineRectLineSegOPPX;
	scAffineRect* m_pAffineRectLineSegXO;
	scAffineRect* m_pAffineRectCircularArcPo;
	scAffineRect* m_pAffineRectCircularArcPx;
	scAffineRect* m_pAffineRectCircularArcPy;
	scAffineRect* m_pAffineRectCircularArcPopp;

	int m_nCaliperNum[3];
	CaliperSearchDirection	m_nSearchDirection[3];
	double m_dSearchLength[3];
	double m_dProjectionLength[3];

	svStd vector<sc2Vector> m_CaplierResultLineSegOY;
	svStd vector<sc2Vector> m_CaplierResultLineSegYOPP;
	svStd vector<sc2Vector> m_CaplierResultLineSegOPPX;
	svStd vector<sc2Vector> m_CaplierResultLineSegXO;
	svStd vector<sc2Vector> m_CaplierResultCircularArcPo;
	svStd vector<sc2Vector> m_CaplierResultCircularArcPx;
	svStd vector<sc2Vector> m_CaplierResultCircularArcPy;
	svStd vector<sc2Vector> m_CaplierResultCircularArcPopp;
	svStd vector<bool> m_bUsedLineSegOY;
	svStd vector<bool> m_bUsedLineSegYOPP;
	svStd vector<bool> m_bUsedLineSegOPPX;
	svStd vector<bool> m_bUsedLineSegXO;
	svStd vector<bool> m_bUsedCircularArcPo;
	svStd vector<bool> m_bUsedCircularArcPx;
	svStd vector<bool> m_bUsedCircularArcPy;
	svStd vector<bool> m_bUsedCircularArcPopp;

	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lSize;
	FitMethod m_fitMethod;

	bool m_bNumOrPrecent;			//true:数量;false:百分比
	long m_nOutLinePrecent;
	long m_nOutLineNum;
	OutLineMethod m_outlineMethod;

	bool m_bSortByScore;
	bool m_bSortReverse;
};

#pragma pack(pop)