#pragma once

#include "svVision3DDef.h"
#include "svZeroPlaneResult.h"
#include "vpImage.h"
#include "vpImageMask.h"

#pragma pack(push,8)

class cpPointCloudGenerateToolBase;
enum ZeroPlaneMethod
{
	eZeroPlaneNone           = 0, // 不指定
	eRegionBestFit           = 1, // 平面基准
	eRegionAveBestFit        = 2, // 平面基准(平均)
	eRegionAveHeight         = 3, // 平均高度基准
	eRealTime3Points         = 4, // 实时三点指定
	eRealTimeMulPoints       = 5, // 实时多点指定
	eUserSpecified           = 6, // 指定值
	eOtherToolZeroPlane		 = 7, // 其他工具起算面
	eOtherToolExtactPlane    = 8, // 其他工具抽取平面	
	eOtherToolInspectPlane   = 9, // 其他工具检测平面
};

class SV_VISION_3D_API scZeroPlaneToolBase
{
public:
	scZeroPlaneToolBase();
	virtual ~scZeroPlaneToolBase();

	// 获取起算面计算类型
	virtual ZeroPlaneMethod GetZeroPlaneMethod() const {return eZeroPlaneNone;}

	// 设置/获取点云生成工具指针
	void SetPointCloudGenerateTool(cpPointCloudGenerateToolBase* pPointCloudGenerateTool);
	cpPointCloudGenerateToolBase* GetPointCloudGenerateTool()const;

	// 计算起算面
	virtual bool Excute(cpImage& image){return false;}

	// 获取起算面计算结果
	scZeroPlaneResult GetZeroPlaneResult() const {return m_ZeroPlaneResult;}

	// 综合roi区域与掩模区域
	static bool MaskMerge(cpImageMask& roi,std::vector<cpImageMask>& mask);

	// 综合检测区域
	static bool InspectRegionMerge(cpImageMask& roi,std::vector<cpImageMask>& mask);

	// 拟合起算面
	static bool FitZeroPlane(const std::vector<double>& vdx,const std::vector<double>& vdy,const std::vector<double>& vdz,scZeroPlaneResult& zeroPlane);
	static bool FitZeroPlane(const std::vector<double>& vdx,const std::vector<double>& vdy,const std::vector<double>& vdz,int nOutPercent,scZeroPlaneResult& zeroPlane);

protected:
	cpPointCloudGenerateToolBase* m_pPointCloudGenerateTool;
	scZeroPlaneResult m_ZeroPlaneResult;
};

#pragma pack(pop)