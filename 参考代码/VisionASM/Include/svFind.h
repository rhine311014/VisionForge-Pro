#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svCircle.h"
#include "svCircularArc.h"
#include "svAffineRect.h"
#include "svCaliper.h"
#include "vpImage.h"

#pragma pack(push,8)

class SV2DMEASURE_API scFindLine
{
public:
	scFindLine();
	~scFindLine();

	//	设置线段参数
	void SetStartXYEndXY(double dStartX,			// 起始点X坐标
						 double dStartY,			// 起始点Y坐标
						 double dEndX,				// 终点X坐标
						 double dEndY);				// 终点Y坐标

	// 获取线段参数
	void GetStartXYEndXY(double& dStartX,			// 起始点X坐标
						 double& dStartY,			// 起始点Y坐标
						 double& dEndX,				// 终点X坐标
						 double& dEndY) const;		// 终点Y坐标

	//	设置线段参数
	void SetStartXYEndXY(const sc2Vector& vStart, 	// 起始点
						 const sc2Vector& vEnd);	// 终点

	// 获取线段参数
	void GetStartXYEndXY(sc2Vector& vStart, 		// 起始点
						 sc2Vector& vEnd) const;	// 终点

	void SetLineSeg(const scLineSeg& lineSeg);		// 设置线段
	scLineSeg GetLineSeg() const;					// 获取线段

	void SetCaliperParams(int	nCaliperNum,						// 设置卡尺数量
						  const scRadian& rSearchDirection,			// 设置卡尺搜索方向
						  double dSearchLength,						// 设置卡尺搜索长度							 
						  double dProjectionLength);				// 设置卡尺投影长度

	void SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void SetCaliperSearchDirection(const scRadian& rSearchDirection);	// 设置卡尺搜索方向
	void SetCaliperSearchLength(double dSearchLength);					// 设置卡尺搜索长度
	void SetCaliperProjectionLength(double dProjectionLength);			// 设置卡尺投影长度

	int GetCaliperNum()					 const;						// 获取卡尺数量
	scRadian GetCaliperSearchDirection() const;						// 获取卡尺搜索方向
	double GetCaliperSearchLength()		 const;						// 获取卡尺搜索长度
	double GetCaliperProjectionLength()	 const;						// 获取卡尺投影长度

	void SetCaliperMasked(svStd vector<bool>& vMasked);				// 设置卡尺是否被掩模覆盖
	void SetCaliperMasked(svStd vector<BOOL>& vMasked);				// 设置卡尺是否被掩模覆盖
	void ClearCaliperMasked();										// 清空卡尺掩没数据

	// 获取第nIndex个卡尺的参数
	void GetCaliper(int nIndex,										// 卡尺序号，序号范围从0到nCaliperNum-1
					double& dCenterX,								// 仿射矩形（卡尺）中心点的X坐标  
					double& dCenterY,								// 仿射矩形（卡尺）中心点的Y坐标 
					double& dSideXLength,							// 仿射矩形（卡尺）X边长度 
					double& dSideYLength,							// 仿射矩形（卡尺）Y边长度 
					scRadian& rRotation,							// 仿射矩形（卡尺）旋转角度（单位弧度）
					scRadian& rSkew)	const;						// 仿射矩形（卡尺）切变角度（单位弧度）
	scAffineRect GetCaliper(int nIndex) const;						// 获取第nIndex个卡尺

	// 设置卡尺属性
	void SetCaliperProperty(double dContrastThreshold,			// 设置卡尺对比度阈值
							EdgePolarity nEdgePolarity,			// 设置卡尺极性
							long lSize);	// 设置卡尺滤波器半宽尺寸
	void SetCaliperProperty(double dContrastThreshold,			// 设置卡尺对比度阈值
							EdgePolarity nEdgePolarity1,		// 设置卡尺边缘1极性
							EdgePolarity nEdgePolarity2,		// 设置卡尺边缘2极性
							long lSize);						// 设置卡尺滤波器半宽尺寸
	void EnableCaliperSortByScore(BOOL bSortByScore);
	void EnableCaliperSortReverse(BOOL bSortReverse);
	void EnableCaliperPeakDetect(BOOL bPeakDetect);
	void EnableLineConstraint(BOOL bEnable, scLine line = scLine(), double disthre = 1.5);

	//单边缘灰度判断参数设置：Forward，沿搜索方向往前判断；Backward，沿搜索方向朝后判断
	void EnableJudgeGrayForward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255, int nJudgeRangeStart = 0, int nJudgeRangeLength = 1);
	void EnableJudgeGrayBackward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255, int nJudgeRangeStart = 0, int nJudgeRangeLength = 1);
	
	//双边缘灰度判断参数设置
	void EnableJudgeGrayInward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255);

	// 获取卡尺属性
	void GetCaliperProperty(double& dContrastThreshold, EdgePolarity& nEdgePolarity, long& lSize);
	void GetCaliperProperty(double& dContrastThreshold, EdgePolarity& nEdgePolarity1, EdgePolarity& nEdgePolarity2, long& lSize);

	// 设置局外点个数
	bool SetOutLineNum(int num, OutLineMethod method = eRobust);
	bool SetOutLinePrecent(int precent, OutLineMethod method = eRobust);	//precent:0~100

	// 设置角度约束
	void SetAngleConstrain(bool bConstrain, scRadian angleConstrain = scRadian(0));

	//设置每个卡尺最多输出几个拟合点
	void SetMaxFitPointNum(int nMaxFitPointNum);
	int  GetMaxFitPointNum();

	//设置使用第几个点进行拟合
	void SetFitPointIndex(int nFitPointIndex);
	int  GetFitPointIndex();

	//设置拟合第几条边(仅对双边缘模式有效)：//0-中心线；1-线1；2-线2
	void SetFitLineIndex(int nFitLineIndex);
	int  GetFitLineIndex();

	//设置是否开启分段拟合(即要求找线工具起始两端的点都参与拟合)
	void SetFit2SegmentEnable(bool bEnable);
	bool GetFit2SegmentEnable();

	// 执行FindLine工具
	bool Execute(const scImageGray& image, scLine& line, double& rms);
	bool Execute(const scImageGray& image, scLineSeg& lineSeg, double& rms);
	bool Execute(cpImage& image, scLine& line, double& rms);
	bool Execute(cpImage& image, scLineSeg& lineSeg, double& rms);

	// 获取第n条线结果
	bool GetResult(int nFitPointIndex, scLine& line, double& rms);
	bool GetResult(int nFitPointIndex, scLineSeg& lineSeg, double& rms);

	// 获取所有拟合点（当nMaxFitPointNum>1时，输出所有找到的卡尺点）
	bool GetAllFitData(svStd vector<sc2Vector>& points);

	// 获取所有拟合直线（dDistanceThre，点到直线最远距离；nMinPoints，直线上点的最少个数；eSortMode，输出多个直线的排序方式；nMaxLines，最多输出直线数量）
	bool GetAllFitLine(double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxLines, 
		svStd vector<scLine>& vLine, svStd vector<double>& vdRms, svStd vector<sc2Vector>& vPoints, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);
	bool GetAllFitLineSeg(double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxLines, 
		svStd vector<scLineSeg>& vLineSeg, svStd vector<double>& vdRms, svStd vector<sc2Vector>& vPoints, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);

	// 获取参与拟合的数据点
	bool GetFitData(svStd vector<sc2Vector>& points);
	bool GetEdgeCenterData(svStd vector<sc2Vector>& points);
	bool GetEdge1Data(svStd vector<sc2Vector>& points);
	bool GetEdge2Data(svStd vector<sc2Vector>& points);
	bool GetUsed(svStd vector<bool>& used);
	bool GetUsed(svStd vector<BOOL>& used);

	// 获取卡尺工具指针
	scCaliperOneEdge* GetCaliperToolOneEdge();
	scCaliperTwoEdge* GetCaliperToolTwoEdge();

private:
	scCaliperOneEdge m_CaliperToolOneEdge;
	scCaliperTwoEdge m_CaliperToolTwoEdge;

	void SetDefaults();	
	void UpdateCaliper();

	scLineSeg		m_LineSeg;		
	scAffineRect*	m_pAffineRect;

	int				m_nCaliperNum;
	scRadian		m_rSearchDirection;
	double			m_dSearchLength;
	double			m_dProjectionLength;

	svStd vector<sc2Vector> m_CaplierResult;
	svStd vector<sc2Vector> m_CaplierResultEdgeCenter;
	svStd vector<sc2Vector> m_CaplierResultEdge1;
	svStd vector<sc2Vector> m_CaplierResultEdge2;

	svStd vector<svStd vector<scCaliperOneEdgeResult>> m_CaplierResultOneEdge;
	svStd vector<svStd vector<scCaliperTwoEdgeResult>> m_CaplierResultTwoEdge;

	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lSize;

	bool m_bSortByScore;
	bool m_bSortReverse;
	bool m_bPeakDetect;
	bool m_bEnableLineConstraint;
	scLine m_Line;
	double m_dDisThre;
	bool m_bJudgeGrayForward;
	int m_nGrayThreLowForward;
	int m_nGrayThreHighForward;
	int m_nJudgeRangeStartForward;
	int m_nJudgeRangeLengthForward;
	bool m_bJudgeGrayBackward;
	int m_nGrayThreLowBackward;
	int m_nGrayThreHighBackward;
	int m_nJudgeRangeStartBackward;
	int m_nJudgeRangeLengthBackward;
	bool m_bJudgeGrayInward;
	int m_nGrayThreLowInward;
	int m_nGrayThreHighInward;

	int m_nMaxFitPointNum;	//每个卡尺最多输出几个拟合点
	int m_nFitPointIndex;	//选第几个点进行拟合直线
	int m_nFitLineIndex;	//0-中心线；1-线1；2-线2

	bool m_bFit2SegmentEnable;

	bool m_bNumOrPrecent;			//true:数量;false:百分比
	long m_nOutLinePrecent;
	long m_nOutLineNum;
	OutLineMethod m_bMethod;
	svStd vector<bool> m_bUsed;

	svStd vector<bool> m_bMasked;

	bool m_bAngleConstrain;
	scRadian m_rAngleConstrain;
};

class SV2DMEASURE_API scFindCircle
{
public:
	scFindCircle();
	~scFindCircle();

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(double dCenterX,			// 圆心的X坐标
											double dCenterY,			// 圆心的Y坐标
											double dRadius,				// 半径
											double dAngleStart,			// 起始角度，值域[-π，π)
											double dAngleSpan);			// 角跨度，值域[-4π，4π)
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(double& dCenterX,			// 圆心的X坐标
											double& dCenterY,			// 圆心的Y坐标
											double& dRadius,			// 半径
											double& dAngleStart,		// 起始角度，值域[-π，π)
											double& dAngleSpan) const;	// 角跨度，值域[-4π，4π)

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(const sc2Vector& vCenter,	// 圆心坐标
											double dRadius,				// 半径
											const scRadian& rAngleStart,// 起始角度，单位弧度，值域[-π，π)
											const scRadian& rAngleSpan);// 张角，值域[-4π，4π)
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(sc2Vector& vCenter,			// 圆心坐标
											double& dRadius,			// 半径
											scRadian& rAngleStart,		// 起始角度，单位弧度，值域[-π，π)
											scRadian& rAngleSpan) const;// 张角，值域[-4π，4π)

	void	SetCircularArc(const scCircularArc& circularArc);			// 设置圆弧
	scCircularArc GetCircularArc() const;								// 获取圆弧

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

	void SetCaliperMasked(svStd vector<bool>& vMasked);				// 设置卡尺是否被掩模覆盖
	void SetCaliperMasked(svStd vector<BOOL>& vMasked);				// 设置卡尺是否被掩模覆盖
	void ClearCaliperMasked();										// 清空卡尺掩没数据

	// 获取第nIndex个卡尺的参数
	void GetCaliper(int	   nIndex,						// 卡尺序号，序号范围从0到nCaliperNum-1
					double& dCenterX,					// 仿射矩形（卡尺）中心点的X坐标  
					double& dCenterY,					// 仿射矩形（卡尺）中心点的Y坐标 
					double& dSideXLength,				// 仿射矩形（卡尺）X边长度 
					double& dSideYLength,				// 仿射矩形（卡尺）Y边长度 
					scRadian& rRotation,				// 仿射矩形（卡尺）旋转角度（单位弧度）
					scRadian& rSkew)	const;			// 仿射矩形（卡尺）切变角度（单位弧度）
	scAffineRect GetCaliper(int nIndex) const;			// 获取第nIndex个卡尺	

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

	//设置每个卡尺最多输出几个拟合点
	void SetMaxFitPointNum(int nMaxFitPointNum);
	int  GetMaxFitPointNum();

	//设置使用第几个点进行拟合
	void SetFitPointIndex(int nFitPointIndex);
	int  GetFitPointIndex();

	//设置拟合第几条边(仅对双边缘模式有效)：//0-中心线；1-线1；2-线2
	void SetFitCircleIndex(int nFitCircleIndex);
	int  GetFitCircleIndex();

	// 执行FindCircle工具
	bool Execute(const scImageGray& image, scCircle& circle, double& rms);
	bool Execute(const scImageGray& image, scCircularArc& circleArc, double& rms);
	bool Execute(cpImage& image, scCircle& circle, double& rms);
	bool Execute(cpImage& image, scCircularArc& circleArc, double& rms);

	// 获取第n条线结果
	bool GetResult(int nFitPointIndex, scCircle& circle, double& rms);
	bool GetResult(int nFitPointIndex, scCircularArc& circleArc, double& rms);

	// 获取所有拟合点（当nMaxFitPointNum>1时，输出所有找到的卡尺点）
	bool GetAllFitData(svStd vector<sc2Vector>& points);

	// 获取所有拟合圆（dDistanceThre，点到圆最远距离；nMinPoints，圆上点的最少个数；eSortMode，输出多个圆的排序方式；nMaxCircles，最多输出圆数量）
	bool GetAllFitCircle(double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxCircles, 
		svStd vector<scCircle>& vCircle, svStd vector<double>& vdRms, svStd vector<sc2Vector>& vPoints, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);
	bool GetAllFitCircularArc(double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxCircles, 
		svStd vector<scCircularArc>& vCircularArc, svStd vector<double>& vdRms, svStd vector<sc2Vector>& vPoints, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);


	// 获取参与拟合的数据点
	bool GetFitData(svStd vector<sc2Vector>& points);
	bool GetEdgeCenterData(svStd vector<sc2Vector>& points);
	bool GetEdge1Data(svStd vector<sc2Vector>& points);
	bool GetEdge2Data(svStd vector<sc2Vector>& points);
	bool GetUsed(svStd vector<bool>& used);
	bool GetUsed(svStd vector<BOOL>& used);

	// 获取卡尺工具指针
	scCaliperOneEdge* GetCaliperToolOneEdge();
	scCaliperTwoEdge* GetCaliperToolTwoEdge();

private:
	scCaliperOneEdge m_CaliperToolOneEdge;
	scCaliperTwoEdge m_CaliperToolTwoEdge;
	
	void SetDefaults();
	void UpdateCaliper();

	scCircularArc m_CircularArc;
	scAffineRect* m_pAffineRect;

	int m_nCaliperNum;
	CaliperSearchDirection	m_nSearchDirection;
	double m_dSearchLength;
	double m_dProjectionLength;

	svStd vector<sc2Vector> m_CaplierResult;
	svStd vector<sc2Vector> m_CaplierResultEdgeCenter;
	svStd vector<sc2Vector> m_CaplierResultEdge1;
	svStd vector<sc2Vector> m_CaplierResultEdge2;

	svStd vector<svStd vector<scCaliperOneEdgeResult>> m_CaplierResultOneEdge;
	svStd vector<svStd vector<scCaliperTwoEdgeResult>> m_CaplierResultTwoEdge;

	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lSize;
	FitMethod m_Method;

	int m_nMaxFitPointNum;	//每个卡尺最多输出几个拟合点
	int m_nFitPointIndex;	//选第几个点进行拟合圆
	int m_nFitCircleIndex;	//0-中心线；1-线1；2-线2

	bool m_bNumOrPrecent;			//true:数量;false:百分比
	long m_nOutLinePrecent;
	long m_nOutLineNum;
	OutLineMethod m_bMethod;
	svStd vector<bool> m_bUsed;

	bool m_bSortByScore;
	bool m_bSortReverse;

	svStd vector<bool> m_bMasked;
};

#pragma pack(pop)