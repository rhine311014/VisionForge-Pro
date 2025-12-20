#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include "svCalibrate.h"
#include "vpImage.h"

#pragma pack(push,8)

class SV2DMEASURE_API scImageWarpProject
{
public:
	scImageWarpProject(const scCalibrateResult& result, const sc2Vector& beforeimagesize);
	~scImageWarpProject();
	
	// 设置/获取标定结果
	bool SetCalibrateResult(const scCalibrateResult& result, const sc2Vector& beforeimagesize);
	scCalibrateResult GetCalibrateResult();
	sc2Vector		  GetBeforeImageSize();
	
	// 执行镜头畸变校正
	bool ExecuteImageWarpProject(const scImageGray& srcimage, scImageGray& dstimage, bool bClipped = true, bool bBilinear = false);
	bool ExecuteImageWarpProject(cpImage& srcimage, cpImage& dstimage, bool bClipped = true, bool bBilinear = false);
	
	// 创建查找表
	void CreateWarpLut();
	
private:
	long  m_lBeforeWidth;
	long  m_lBeforeHeight;
	scCalibrateResult m_CalibrateResult;

	long*	m_pProjectWarpIndex;
	long	m_nProjectWarpWidth;
	long	m_nProjectWarpHeight;
};

#pragma pack(pop)