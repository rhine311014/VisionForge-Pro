#pragma once

#include "svDef.h"
#include "svAngle.h"
#include "svImage.h"
#include "vpImage.h"
#include "svAffineRect.h"
#include "svCaliperResult.h"

#pragma pack(push,8)

class SV2DMEASURE_API scCaliperOneEdge  
{
public:
	scCaliperOneEdge();
	virtual ~scCaliperOneEdge();

	void EnableSortByScore(BOOL bSortByScore);
	void EnableSortReverse(BOOL bSortReverse);
	void EnablePeakDetect(BOOL bPeakDetect);
	void EnableJudgeGrayForward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255, int nJudgeRangeStart = 0, int nJudgeRangeLength = 1);
	void EnableJudgeGrayBackward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255, int nJudgeRangeStart = 0, int nJudgeRangeLength = 1);
	
public:
	// 设置\获取卡尺极性
	void SetEdgePolarity(EdgePolarity nEdgePolarity);
	EdgePolarity GetEdgePolarity();

	// 设置\获取卡尺对比度阈值
	void SetContrastThreshold(double dContrastThreshold);
	double GetContrastThreshold();

	// 设置\获取滤波器半宽尺寸
	void SetFilterHalfSize(long lSize);
	long GetFilterHalfSize();

	// 设置\获取仿射矩形采样区域
	void SetSampleAffineRect(double dbCenterX, double dbCenterY, double dbSideXLength, double dbSideYLength, scRadian rotation, scRadian skew);
	void SetSampleAffineRect(scAffineRect& affrect);
	scAffineRect GetSampleAffineRect();

	// 设置\获取最大输出模式个数
	void	SetMaxResultNum(long num);
	long	GetMaxResultNum();

	// 设置\获取对比度评价函数
	void	EnableFunctionContrast(bool bEnable);
	void	SetFunctionContrast(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionContrastEnabled();
	void	GetFunctionContrast(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取中心距离评价函数
	void	EnableFunctionCenterDistance(bool bEnable);
	void	SetFunctionCenterDistance(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterDistanceEnabled();
	void	GetFunctionCenterDistance(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取中心偏移评价函数
	void	EnableFunctionCenterOffset(bool bEnable);
	void	SetFunctionCenterOffset(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterOffsetEnabled();
	void	GetFunctionCenterOffset(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取极性灰度评价函数
	void	EnableFunctionPolarity(bool bEnable);
	void	SetFunctionPolarity(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionPolarityEnabled();
	void	GetFunctionPolarity(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取亮极灰度评价函数
	void	EnableFunctionLightPolarity(bool bEnable);
	void	SetFunctionLightPolarity(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionLightPolarityEnabled();
	void	GetFunctionLightPolarity(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取暗极灰度评价函数
	void	EnableFunctionDarkPolarity(bool bEnable);
	void	SetFunctionDarkPolarity(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionDarkPolarityEnabled();
	void	GetFunctionDarkPolarity(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 执行卡尺工具
	bool	Execute(const scImageGray& image);
	bool	Execute(cpImage& image);

	// 获取卡尺工具结果的个数
	long	GetResultNum();

	// 获取第i个卡尺工具结果(默认为第一个结果，即评价分数最高的结果)
	scCaliperOneEdgeResult*	GetCaliperResult(long index = 0);

	//获取投影(滤波)数据
	long	GetProjectLength() { return m_lProjectLength; };
	double*	GetProjectValues() { return m_pdbProjectValues; };
	double*	GetFilterValues()  { return m_pdbFilterValues; };

	// 清除卡尺工具结果
	void	ClearResult();

private:
	void*					m_pCaliper;
	scAffineRect			m_rcAffineRect;
	scCaliperOneEdgeResult*	m_pCapResult;
	long					m_lCapResultNum;
	long					m_lProjectLength;
	double*					m_pdbProjectValues;
	double*					m_pdbFilterValues;
};

class SV2DMEASURE_API scCaliperTwoEdge
{
public:
	scCaliperTwoEdge();
	virtual ~scCaliperTwoEdge();

public:

	void EnableJudgeGrayInward(BOOL bEnable, int nGrayThreLow = 0, int nGrayThreHigh = 255);

	// 设置\获取卡尺极性
	void SetEdgePolarity(EdgePolarity nEdge1Polarity, EdgePolarity nEdge2Polarity);
	EdgePolarity GetEdge1Polarity();
	EdgePolarity GetEdge2Polarity();

	// 设置\获取双边缘宽度
	void SetEdgePairWidth(double dEdgePairWidth);
	double GetEdgePairWidth();

	// 设置\获取卡尺对比度阈值
	void SetContrastThreshold(double dContrastThreshold);
	double GetContrastThreshold();

	// 设置\获取滤波器半宽尺寸
	void SetFilterHalfSize(long lSize);
	long GetFilterHalfSize();

	// 设置\获取仿射矩形采样区域
	void SetSampleAffineRect(double dbCenterX, double dbCenterY, double dbSideXLength, double dbSideYLength, scRadian rotation, scRadian skew);
	void SetSampleAffineRect(scAffineRect& affrect);
	scAffineRect GetSampleAffineRect();

	// 设置\获取最大输出模式个数
	void	SetMaxResultNum(long num);
	long	GetMaxResultNum();

	// 设置\获取对比度评价函数
	void	EnableFunctionContrast(bool bEnable);
	void	SetFunctionContrast(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionContrastEnabled();
	void	GetFunctionContrast(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取中心距离评价函数
	void	EnableFunctionCenterDistance(bool bEnable);
	void	SetFunctionCenterDistance(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterDistanceEnabled();
	void	GetFunctionCenterDistance(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取中心偏移评价函数
	void	EnableFunctionCenterOffset(bool bEnable);
	void	SetFunctionCenterOffset(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterOffsetEnabled();
	void	GetFunctionCenterOffset(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取归一化中心距离评价函数
	void	EnableFunctionCenterDistanceNorm(bool bEnable);
	void	SetFunctionCenterDistanceNorm(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterDistanceNormEnabled();
	void	GetFunctionCenterDistanceNorm(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取归一化中心偏移评价函数
	void	EnableFunctionCenterOffsetNorm(bool bEnable);
	void	SetFunctionCenterOffsetNorm(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionCenterOffsetNormEnabled();
	void	GetFunctionCenterOffsetNorm(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取归一化宽度评价函数
	void	EnableFunctionWidthNorm(bool bEnable);
	void	SetFunctionWidthNorm(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionWidthNormEnabled();
	void	GetFunctionWidthNorm(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取归一化宽度差评价函数
	void	EnableFunctionWidthDiffNorm(bool bEnable);
	void	SetFunctionWidthDiffNorm(double X0, double X1, double XC, double Y0, double Y1, 
		double X0H, double X1H, double XCH, double Y0H, double Y1H);
	bool	IsFunctionWidthDiffNormEnabled();
	void	GetFunctionWidthDiffNorm(double& X0, double& X1, double& XC, double& Y0, double& Y1, 
		double& X0H, double& X1H, double& XCH, double& Y0H, double& Y1H);

	// 设置\获取绝对归一化宽度差评价函数
	void	EnableFunctionWidthDiffNormAbs(bool bEnable);
	void	SetFunctionWidthDiffNormAbs(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionWidthDiffNormAbsEnabled();
	void	GetFunctionWidthDiffNormAbs(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 设置\获取跨中心分布评价函数
	void	EnableFunctionStraddle(bool bEnable);
	bool	IsFunctionStraddleEnabled();

	// 设置\获取中间平均灰度评价函数
	void	EnableFunctionAverageGray(bool bEnable);
	void	SetFunctionAverageGray(double X0, double X1, double XC, double Y0, double Y1);
	bool	IsFunctionAverageGrayEnabled();
	void	GetFunctionAverageGray(double& X0, double& X1, double& XC, double& Y0, double& Y1);

	// 执行卡尺工具
	bool	Execute(const scImageGray& image);
	bool	Execute(cpImage& image);

	// 获取卡尺工具结果的个数
	long	GetResultNum();

	// 获取第i个卡尺工具结果(默认为第一个结果，即评价分数最高的结果)
	scCaliperTwoEdgeResult*	GetCaliperResult(long index = 0);

	//获取投影(滤波)数据
	long	GetProjectLength() { return m_lProjectLength; };
	double*	GetProjectValues() { return m_pdbProjectValues; };
	double*	GetFilterValues()  { return m_pdbFilterValues; };

	// 清除卡尺工具结果
	void	ClearResult();

private:
	void*					m_pCaliper;
	scAffineRect			m_rcAffineRect;
	scCaliperTwoEdgeResult*	m_pCapResult;
	long					m_lCapResultNum;
	long					m_lProjectLength;
	double*					m_pdbProjectValues;
	double*					m_pdbFilterValues;
};

#pragma pack(pop)
