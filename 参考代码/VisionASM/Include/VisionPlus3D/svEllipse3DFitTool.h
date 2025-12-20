#pragma once

#include <vector>
#include "vpImageMask.h"
#include "vpPointCloudGenerateToolDefault.h"
#include "svEllipse3D.h"
#include "svVision3DDef.h"

#pragma pack(push,8)

class SV_VISION_3D_API scEllipse3DFitTool
{
public:
	scEllipse3DFitTool(void);
	~scEllipse3DFitTool(void);

	//设置/获取采样方法
	void SetSamplingMethod(SamplingMethod method){ m_SamplingMethod = method; };
	SamplingMethod GetSamplingMethod() const { return m_SamplingMethod; };

	//设置/获取采样间隔参数
	void SetSamplingParam(int nX, int nY);
	void GetSamplingParam(int& nX, int& nY) const;

	//设置/获取平面拟合方法
	void SetFitEllipseMethod(FitMethod3D method) {m_FitMethod = method; };
	FitMethod3D GetFitEllipseMethod() const { return m_FitMethod; };

	//设置/获取是否开启Ransac方法的自动距离阈值
	void SetRansacAutoThreEnable(bool bEnable) { m_bRansacAutoThreEnable = bEnable; };
	bool GetRansacAutoThreEnable() const { return m_bRansacAutoThreEnable; };

	//设置/获取是否开启Tukey方法的自动距离阈值
	void SetTukeyAutoThreEnable(bool bEnable) { m_bTukeyAutoThreEnable = bEnable; };
	bool GetTukeyAutoThreEnable() const { return m_bTukeyAutoThreEnable; };

	//设置/获取局外点百分比
	void SetOutLinePercent(int nOutPer) { m_nOutPer = nOutPer; };
	int GetOutLinePercent() const { return m_nOutPer; };

	//设置/获取正解概率
	void SetProbSolution(double dProbSolution) {m_dProbSolution = dProbSolution; };
	double GetProbSolution() const {return m_dProbSolution; }; 

	//设置/获取最大迭代次数
	void SetMaxIterNum(int iter) {m_nIter = iter; };
	int GetMaxIterNum() const {return m_nIter; };

	//设置/获取收敛精度
	void SetAccuracy(double accuracy) {m_dAccuracy = accuracy; };
	double GetAccuracy() const { return m_dAccuracy; };

	//设置/获取点云
	void SetPointCloudGenerateTool(cpPointCloudGenerateToolDefault* pPointCloudGenerateTool) {m_pPointCloudGenerateTool = pPointCloudGenerateTool; };
	cpPointCloudGenerateToolDefault* GetPointCloudGenerateTool()const {return m_pPointCloudGenerateTool; };

	//根据深度图拟合平面，输出结果图像
	bool Execute(cpImage img,scShape* pROI,scEllipse3D& Ellipse3DResult,double& dRms,cpImageMask& ResultImage);

	//根据点云数据拟合平面，输出是否参与拟合的点
	bool Execute(const svStd vector<sc3Vector>& vPoints,scEllipse3D& Ellipse3DResult,double& dRms,svStd vector<bool>& vbUsed);

private:

	SamplingMethod m_SamplingMethod;
	int m_nX, m_nY;
	FitMethod3D m_FitMethod;
	bool m_bRansacAutoThreEnable, m_bTukeyAutoThreEnable;
	cpPointCloudGenerateToolDefault* m_pPointCloudGenerateTool;
	int m_nOutPer;
	double m_dProbSolution;
	int m_nIter;
	double m_dAccuracy;
};

#pragma pack(pop)
