#pragma once

#include "svDef.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

enum HomographyDOF
{
	eAuto=0,
	eScaleRotate,
	eScaleXYRotateXY,
	eProject,
	eScaleRotateMirror,
};

enum AlignMethod
{
	eMinDistance = 0,
	eAverageDistance = 1,
};

// 计算单应矩阵
SV2DMEASURE_API bool sfGetHomography(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, double h[9], HomographyDOF dof = eAuto);

// 计算单应矩阵(局外点)，不支持dof = eAuto
SV2DMEASURE_API bool sfGetHomographyOutLine(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, double h[9], int outcount, svStd vector<bool>& used, HomographyDOF dof);

// 单应矩阵投影
SV2DMEASURE_API sc2Vector sfHomographyProject(const sc2Vector srcpt, const double h[9]);

// 单应矩阵逆投影
SV2DMEASURE_API sc2Vector sfHomographyInverseProject(const sc2Vector dstpt, const double h[9]);

SV2DMEASURE_API bool sfGetHomographyMaxError(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, const double h[9], double& dMaxError, svStd vector<sc2Vector>& offset);
SV2DMEASURE_API bool sfGetHomographyMaxErrorInverseProject(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, const double h[9], double& dMaxError, svStd vector<sc2Vector>& offset);

// 计算刚体变换(dTx,dTy,radTd)
SV2DMEASURE_API bool sfGetRigidTransformation(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, double& dTx, double& dTy, scDegree& degTd, const sc2Vector ptOrigin = sc2Vector(0,0), AlignMethod method = eAverageDistance);//平台轴运动量,以指定的平台旋转轴位置ptOrigin为旋转中心计算平移偏差(dTx,dTy)
SV2DMEASURE_API bool sfGetRigidTransformationProduct(const svStd vector<sc2Vector>& srcpts, const svStd vector<sc2Vector>& dstpts, double& dTx, double& dTy, scDegree& degTd, AlignMethod method = eAverageDistance);	//产品偏移量,以对象srcpts的质心为旋转中心计算平移偏差(dTx,dTy)

// 旋转中心标定求解(多位置同时进行)
// points	输入：所有参与标定计算的点，包括位置1、位置2、...位置n的所有旋转标定点（点数等于nums中各元素之和）
// nums		输入：每个位置参与旋转标定的点的个数(不少于3)
// center	输出：旋转中心坐标
// rms		输出：每个点的计算误差，size与points相同
// dMaxRms	输出：最大计算误差
SV2DMEASURE_API bool sfGetRotationCenter(const svStd vector<sc2Vector>& points, const svStd vector<int>& nums, 
	sc2Vector& center, double& dMaxRms);

#pragma pack(pop)
