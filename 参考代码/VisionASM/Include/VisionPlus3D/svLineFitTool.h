#pragma once

#include "svDef.h"
#include "svDef3D.h"
#include "svVector.h"
#include "svLine3D.h"
#include "vpPointCloudGenerateToolBase.h"
#include "svVision3DDef.h"
#pragma pack(push,8)

class SV_VISION_3D_API scLineFitTool
{
public:
	scLineFitTool();
	~scLineFitTool();

	//设置/获取平面拟合方法
	void SetFitPlaneMethod(FitMethod3D method) {m_FitMethod = method; };
	FitMethod3D GetFitPlaneMethod() const { return m_FitMethod; }
	
	//设置/获取是否开启Ransac方法的自动距离阈值
	void EnableRansacAutoThreshold(bool bEnable) { m_bRansacAutoThresh = bEnable; };
	bool IsRansacAutoThresholdEnable() const { return m_bRansacAutoThresh; };

	//设置/获取是否开启Tukey方法的自动距离阈值
	void EnableTukeyAutoThreshold(bool bEnable) { m_bTukeyAutoThresh = bEnable; };
	bool IsTukeyAutoThresholdEnable() const { return m_bTukeyAutoThresh; };

	//设置/获取局外点百分比
	void SetOutLinePercent(int nOutPercent) { m_nOutPercent = nOutPercent; };
	int GetOutLinePercent() const { return m_nOutPercent; };

	//设置/获取正解概率
	void SetProbSolution(double dProbSolution) {m_dProbSolution = dProbSolution; };
	double GetProbSolution() const {return m_dProbSolution; }; 

	//设置/获取最大迭代次数
	void SetMaxIterNum(int nIter) {m_nIter = nIter; };
	int GetMaxIterNum() const {return m_nIter; };

	//设置/获取收敛精度
	void SetAccuracy(double dAccuracy) {m_dAccuracy = dAccuracy; };
	double GetAccuracy() const { return m_dAccuracy; };

	//根据点云数据拟合平面，输出是否参与拟合的点
	bool Execute(const std::vector<sc3Vector>& vPoints, scLine3D& resLine, double& dRms, std::vector<bool>& vbUsed);
	
private:
	FitMethod3D m_FitMethod;			// 拟合方法
	int			m_nOutPercent;			// 局外点百分比	
	bool		m_bRansacAutoThresh;	// Ransac自动距离阈值
	bool		m_bTukeyAutoThresh;		// Tukey自动距离阈值
	double		m_dProbSolution;		// 正解概率
	int			m_nIter;				// 迭代次数
	double		m_dAccuracy;			// 收敛精度
};


#pragma pack(pop)