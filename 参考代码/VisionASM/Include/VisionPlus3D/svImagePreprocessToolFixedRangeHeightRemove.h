#pragma once
#include "svImagePreprocessToolBase.h"
#include "vpPointCloudGenerateToolBase.h"
#pragma pack(push,8)


// 固定范围高度去除
class SV_VISION_3D_API scImagePreprocessToolFixedRangeHeightRemove : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolFixedRangeHeightRemove();
	virtual ~scImagePreprocessToolFixedRangeHeightRemove();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessFixedRangeHeightRemove;}  // 获取预处理类型

	// 设置/获取点云生成工具指针
	void SetPointCloudGenerateTool(cpPointCloudGenerateToolBase* pPointCloudGenerateTool);
	cpPointCloudGenerateToolBase* GetPointCloudGenerateTool()const;

	//设置/获取范围类型 0 - 高度范围， 1 - 像素范围
	void SetRangeType(int nType);
	int GetRangeType()const;

	//设置/获取范围
	void SetRange(double dMin,double dMax);
	void GetRange(double& dMin,double& dMax) const;


	// 执行预处理
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);

private:
	cpPointCloudGenerateToolBase* m_pPointCloudGenerateTool;
	int		m_nRangeType;		// 范围类型 0 - 高度范围， 1 - 像素范围
	double	m_dMin;				// 范围最小值，高度（mm）,像素（Pixel）
	double	m_dMax;				// 范围最大值，高度（mm）,像素（Pixel）
};

#pragma pack(pop)