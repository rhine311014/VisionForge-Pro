#pragma once

#include "svDef.h"
#include "svEllipse.h"
#include "svEllipseArc.h"
#include "svAffineRect.h"
#include "svCaliper.h"
#include "vpImage.h"

#pragma pack(push,8)

class SV2DMEASURE_API scFindEllipse
{
public :
	scFindEllipse();
	~scFindEllipse();

	//设置椭圆弧参数
	void SetCenterRadiiAngleStartAngleEnd(double dCenterX,        // 椭圆心X
		double dCenterY,        // 椭圆新Y
		double dRadiusX,        // X轴半径
		double dRadiusY,        // Y轴半径
		double rRotation,       // 旋转角度
		double dAngleStart,     // 起始角度
		double dAngleSpan);     // 角跨度

	void GetCenterRadiiAngleStartAngleEnd(double& dCenterX,        // 椭圆心X
		double& dCenterY,        // 椭圆新Y
		double& dRadiusX,        // X轴半径
		double& dRadiusY,        // Y轴半径
		double& dRotation,       // 旋转角度
		double& dAngleStart,     // 起始角度
		double& dAngleSpan)const;     // 角跨度

	void SetCenterRadiiAngleStartAngleEnd(sc2Vector& vCenter,      // 椭圆心
		sc2Vector  vRadii,       // 半长轴
		scRadian&  rRotation,    // 旋转角度
		scRadian&  rAngleStart,  // 起始角度
		scRadian&  rAngleSpan);  // 角跨度

	void GetCenterRadiiAngleStartAngleEnd(sc2Vector& vCenter,      // 椭圆心
		sc2Vector&  vRadii,      // 半长轴
		scRadian&  rRotation,    // 旋转角度
		scRadian&  rAngleStart,  // 起始角度
		scRadian&  rAngleSpan)const;  // 角跨度

	void SetEllipseArc(const scEllipseArc& ellipseArc);            // 设置椭圆弧
	scEllipseArc GetEllipseArc() const;                            // 获取椭圆弧

	void SetCaliperParams(int nCaliperNum,							// 设置卡尺数量
		CaliperSearchDirection nDirection,		// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);	            // 设置卡尺投影长度

	void	SetCaliperNum(int nCaliperNum);							// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);			// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);	// 设置卡尺投影长度

	int		GetCaliperNum()					const;			        // 获取卡尺数量
	int		GetCaliperSearchDirection()		const;			        // 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;			        // 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			        // 获取卡尺投影长度

	void SetCaliperMasked(svStd vector<bool>& vMasked);				// 设置卡尺是否被掩模覆盖
	void SetCaliperMasked(svStd vector<BOOL>& vMasked);				// 设置卡尺是否被掩模覆盖
	void ClearCaliperMasked();										// 清空卡尺掩没数据

	// 获取第nIndex个卡尺的参数
	void	GetCaliper(int	nIndex,                                 // 卡尺序号，序号范围从0到nCaliperNum-1
		double& dCenterX,					        // 仿射矩形（卡尺）中心点的X坐标  
		double& dCenterY,					        // 仿射矩形（卡尺）中心点的Y坐标 
		double& dSideXLength,				        // 仿射矩形（卡尺）X边长度 
		double& dSideYLength,				        // 仿射矩形（卡尺）Y边长度 
		scRadian& rRotation,				            // 仿射矩形（卡尺）旋转角度（单位弧度）
		scRadian& rSkew)	const;			            // 仿射矩形（卡尺）切变角度（单位弧度）

	scAffineRect GetCaliper(int nIndex) const;				        // 获取第nIndex个卡尺

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
	bool SetOutLineNum(int num, OutLineMethod method = eRansacRobust);
	bool SetOutLinePrecent(int precent, OutLineMethod method = eRansacRobust);	//precent:0~100

	// 执行FindCircle工具
	bool Execute(const scImageGray& image, scEllipse& ellipse, double& rms);
	bool Execute(const scImageGray& image, scEllipseArc& ellipseArc, double& rms);
	bool Execute(cpImage& image, scEllipse& ellipse, double& rms);
	bool Execute(cpImage& image, scEllipseArc& ellipseArc, double& rms);

	// 获取参与拟合的数据点
	bool GetFitData(svStd vector<sc2Vector>& points);
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

	scEllipseArc m_EllipseArc;
	scAffineRect* m_pAffineRect;

	int m_nCaliperNum;
	CaliperSearchDirection	m_nSearchDirection;
	double m_dSearchLength;
	double m_dProjectionLength;

	svStd vector<sc2Vector> m_CaplierResult;

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
	svStd vector<bool> m_bUsed;

	bool m_bSortByScore;
	bool m_bSortReverse;

	svStd vector<bool> m_bMasked;
};

#pragma pack(pop)