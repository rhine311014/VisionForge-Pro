#pragma once

#include "svDef3D.h"
#include <vector>
#include "vpImage.h"

class scPlane;
class cpPointCloudGenerateToolBase;
#pragma pack(push,8)

// 起算面结果
class SVGEOMETRICS_API_3D scZeroPlaneResult
{
public:
	scZeroPlaneResult();
	scZeroPlaneResult(const scZeroPlaneResult& result);
	~scZeroPlaneResult();

	scZeroPlaneResult& operator=(const scZeroPlaneResult& result);
	bool operator==(const scZeroPlaneResult& result)const;

	// dZeroPlaneZ = dz - m_dSlopeX * dx - m_dSlopY * dy - m_dInterceptZ
	bool Transform(double dx,double dy,double dz,double& dZeroPlaneZ)const;
	bool Transform(const std::vector<double>& vdx,const std::vector<double>& vdy,const std::vector<double>& vdz,std::vector<double>& vdZeroPlaneZ)const;

	// dZeroPlaneZ = dz - m_dSlopeX * dx - m_dSlopY * dy - m_dInterceptZ - dOffset
	bool Transform(double dx,double dy,double dz,double dOffset,double& dZeroPlaneZ)const;
	bool Transform(const std::vector<double>& vdx,const std::vector<double>& vdy,const std::vector<double>& vdz,double dOffset,std::vector<double>& vdZeroPlaneZ)const;

	// dZeroPlaneH = dZeroPlaneZ / sqrt(1.0 + (m_dSlopeX / ux)^2 + (m_dSlopeY / uy)^2);
	// if |dZeroPlaneH| < dNoisCut dZeroPlaneZ = 0.0  dZeroPlaneH = 0.0
	bool TransformZToH(double ux,double uy,double dNoisCut,double& dZeroPlaneZ,double& dZeroPlaneH)const;
	bool TransformZToH(double ux,double uy,double dNoisCut,std::vector<double>& vdZeroPlaneZ,std::vector<double>& vdZeroPlaneH)const;

	bool TransformToZeroPlane(cpImage& src,double dOffset,double dNoisCut,cpPointCloudGenerateToolBase* pPointCloudGenerateTool,cpImage& dst)const;

	// 转换为一般平面，X倾斜、Y倾斜无量纲
	scPlane TransformPlane()const;

	// 转换为一般平面，X倾斜、Y倾斜量纲为mm/pix
	scPlane TransformPlane(cpPointCloudGenerateToolBase* pPointCloudGenerateTool);

public:
	double m_dSlopeX;     // X倾斜 (mm/pix)
	double m_dSlopeY;     // Y倾斜 (mm/pix)
	double m_dInterceptZ; // Z截距 (mm)
};

#pragma pack(pop)
