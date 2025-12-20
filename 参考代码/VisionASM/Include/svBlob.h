#pragma once

#include "svDef.h"
#include "svShape.h"
#include "svImage.h"
#include "vpImage.h"
#include "svBlobSceneDescription.h"
#include <vector>

#pragma pack(push,8)

class SVBLOB_API scBlobTool
{
public:
	scBlobTool();
	virtual ~scBlobTool();

	//分割参数
	void SetSegmentationNone(BYTE scaleVal);
	void SetSegmentationMap(const BYTE byMapArray[256], BYTE scaleVal);
	void SetSegmentationHardThresh(BYTE thresh, bool invert=false);
	void SetSegmentationSoftThresh(BYTE loThresh, BYTE hiThresh, BYTE softness, bool invert=false);
	void SetSegmentationThreshImage(const BYTE byPerMapArray[256], const scImageGray threshimage, const BYTE byPostMapArray[256], BYTE scaleVal);
	void SetSegmentationThreshImage(const BYTE byPerMapArray[256], cpImage threshimage, const BYTE byPostMapArray[256], BYTE scaleVal);

	//掩模设置
	bool Mask(const scImageGray& maskimage);
	bool Mask(cpImage& maskimage);
	void SetMaskOffset(long lOffsetX = 0, long lOffsetY = 0);
	void ClearMask();

	//形态学操作
	void Morph(const std::vector<MorphOp>& m);

	//连通性参数
	void ConnectivityType(scBlobSceneDescription::Analysis c);
	void ConnectivityCleanup(scBlobSceneDescription::ConnectCleanup c);
	void ConnectivityMinPels(int m);

	//参数计算设置
	void EnableComputeInertia(bool bEnable = true);
	void EnableComputeContour(bool bEnable = true);

	//结果显示坐标系设置
	void EnableClientTransform(bool bTransform = false, double dOriginX = 0, double dOriginY = 0, double dTranX = 0, double dTranY = 0, double dAngle = 0);

	//执行连通性分析
	bool Execute(const scImageGray& image, const scShape* roi = NULL);
	bool Execute(cpImage& image, const scShape* roi = NULL);
	bool Execute(cpImage& image, unsigned short usTherLow, unsigned short usTherHigh, bool invert, const scShape* roi = NULL);

	//获取连通性分析结果
	scBlobSceneDescription* GetBSD();

private:
	void* m_hBlobTool;
};

#pragma pack(pop)
