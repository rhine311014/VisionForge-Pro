#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include "svCalibrate.h"
#include "vpImage.h"

#pragma pack(push,8)

class SV2DMEASURE_API scImageWarp
{
public:
	scImageWarp(const scCalibrateResult& result, const sc2Vector& imagesize, bool bCreatAllLut = true);
	~scImageWarp();

	// 设置/获取标定结果
	bool SetCalibrateResult(const scCalibrateResult& result, const sc2Vector& imagesize, bool bCreatAllLut = true);
	scCalibrateResult GetCalibrateResult();
	sc2Vector		  GetImageSize();

	// 执行镜头畸变校正
	bool ExecuteLensWarp(const scImageGray& srcimage, scImageGray& dstimage, bool bClip = false);
	bool ExecuteLensWarp(const scImageRGB& srcimagergb, scImageRGB& dstimagergb, bool bClip = false);
	bool ExecuteLensWarp(cpImage& srcimage, cpImage& dstimage, bool bClip = false);
	bool ExecuteLensWarpPoints(const svStd vector<sc2Vector>& inpoints, svStd vector<sc2Vector>& outpoints, bool bClip = false);

	// 执行投影和镜头畸变校正
	bool ExecuteProjectAndLensWarp(const scImageGray& srcimage, scImageGray& dstimage, bool bWarpRotate = true);
	bool ExecuteProjectAndLensWarp(const scImageRGB& srcimagergb, scImageRGB& dstimagergb, bool bWarpRotate = true);
	bool ExecuteProjectAndLensWarp(cpImage& srcimage, cpImage& dstimage, bool bWarpRotate = true);
	bool ExecuteProjectAndLensWarpPoints(const svStd vector<sc2Vector>& inpoints, svStd vector<sc2Vector>& outpoints, bool bWarpRotate = true);

	// 创建查找表
	void CreateLensWarpLut();
	void CreateLensWarpLutClipped();
	void CreateProjectAndLensWarpLut();
	void CreateProjectAndLensWarpLutNoRotateWarp();
	
	//是否创建查找表
	bool IsCreateLensWarpLut();
	bool IsCreateLensWarpLutClipped();
	bool IsCreateProjectAndLensWarpLut();
	bool IsCreateProjectAndLensWarpLutNoRotateWarp();

private:
	void* m_hWarp;
	void* m_hUndistort;
	long  m_lWidth;
	long  m_lHeight;
	scCalibrateResult m_CalibrateResult;
};

#pragma pack(pop)