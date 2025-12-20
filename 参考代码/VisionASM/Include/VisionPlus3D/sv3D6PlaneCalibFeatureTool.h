#pragma once

#include "svVision3DDef.h"
#include <vector>
#include "svVector.h"
#include "svMaskAffineRect.h"
#include "svZeroPlaneResult.h"
#include "vpPointCloudGenerateToolBase.h"

#pragma pack(push,8)

class SV_CALIBRATE_3D_API sc3D6PlaneCalibFeatureTool
{
public:
	sc3D6PlaneCalibFeatureTool(void);
	~sc3D6PlaneCalibFeatureTool(void);

	enum FitMethod
	{
		eFitMultiPlane,		// 多平面拟合
		eFitRegionSegment	// 区域分割平面拟合
	};

	enum SortMode
	{
		eClockwise,			// 顺时针排序
		eCounterClockwise	// 逆时针排序
	};

	//设置/获取采样方法
	void SetSamplingMethod(SamplingMethod method){ m_SamplingMethod = method;};
	SamplingMethod GetSamplingMethod() const { return m_SamplingMethod; };

	//设置/获取采样间隔参数
	void SetSamplingParam(int nX, int nY) {m_nX = nX; m_nY = nY;};
	void GetSamplingParam(int& nX, int& nY) const {nX = m_nX; nY = m_nY;}

	//设置/获取拟合方法
	void SetFitMethod(FitMethod method) {m_nFitMethod = method;}
	FitMethod GetFitMethod() const {return m_nFitMethod;}

	//设置/获取距离阈值
	void SetDistanceThreshold(double dThreshold) {m_dThreshold = dThreshold;}
	double GetDistanceThreshold() const { return m_dThreshold; }

	//设置/获取拟合平面最小点数
	void SetMinPoint(int nCount) {m_nMinPoint = nCount;}
	int GetMinPoint() const { return m_nMinPoint;}

	//设置/获取标定块侧面排序方法
	void SetSortMode(SortMode mode) {m_nSortMode = mode;}
	SortMode GetSortMode() const {return m_nSortMode;}

	//设置/获取点云
	void SetPointCloudGenerateTool(cpPointCloudGenerateToolBase* pPointCloudGenerateTool) {m_pPointCloudGenerateTool = pPointCloudGenerateTool; };
	cpPointCloudGenerateToolBase* GetPointCloudGenerateTool()const {return m_pPointCloudGenerateTool; };

	bool Execute(const cpImage& img, scAffineRect rcTop, scAffineRect rcBevel, scAffineRect rcBottom, std::vector<sc3Vector>& vPoint);

	//获取平面结果和Rms误差
	void GetPlaneResult(std::vector<scZeroPlaneResult>& vPlane, std::vector<double>& vdRms)const;

public:
	bool GetSampleValidPoint(const cpImage& img, const cpImageMask& mask, std::vector<sc3Vector>& vPoint);
	bool FitMultiPlane(const cpImage& img, scAffineRect rcTop, scAffineRect rcBevel, scAffineRect rcBottom, std::vector<scZeroPlaneResult>& vFitRes, std::vector<std::vector<sc3Vector>>& vFitPoint, std::vector<double>& vdRms);
	bool FitRegionSegment(const cpImage& img, scAffineRect rcTop, scAffineRect rcBevel, scAffineRect rcBottom, std::vector<scZeroPlaneResult>& vFitRes, std::vector<std::vector<sc3Vector>>& vFitPoint, std::vector<double>& vdRms);
	bool MatchPlane(const std::vector<scZeroPlaneResult>& vFitRes, const std::vector<double>& vdRms, const std::vector<std::vector<sc3Vector>>& vFitPoint, std::vector<scZeroPlaneResult>& vMatchRes, std::vector<double>& vdRmsRes);
	bool GetPlaneIntersection(const std::vector<scZeroPlaneResult>& vMatchRes, std::vector<sc3Vector>& vVertex);
private:
	SamplingMethod	m_SamplingMethod;	// 采样方法
	int				m_nX, m_nY;			// 采样大小
	FitMethod		m_nFitMethod;		// 拟合方法
	double			m_dThreshold;		// 距离阈值
	int				m_nMinPoint;		// 拟合平面最小点数
	SortMode		m_nSortMode;		// 排序方法
	std::vector<double>	m_vdRms;		// 匹配平面Rms误差
	std::vector<scZeroPlaneResult>	m_vPlane;	// 匹配平面
	cpPointCloudGenerateToolBase* m_pPointCloudGenerateTool;
};

#pragma pack(pop)