//vsAlignBase.h
#pragma once

#include "svVector.h"
#include "svMatrix.h"
#include "vsAlignBaseDef.h"

#pragma pack(push,8)

// 4X4矩阵求逆
VS_ALIGNBASE_API int MatrixInvert(double A[4][4], int n);

// 2X2矩阵乘以点(2X1的向量)，得到新的点(2X1的向量)
VS_ALIGNBASE_API sc2Vector vf2MatrixProject(const sc2Vector vSrcPt, const sc2Matrix& matrix);


// 对点进行变换，先旋转然后平移得到新的点
VS_ALIGNBASE_API sc2Vector vfGetTransformRotateShiftPoint(
	sc2Vector vSrcPt, scDegree degRotate, double dShiftX, double dShiftY, sc2Vector vRotateCenter = sc2Vector(0,0));

// 对点进行变换，沿着设定方向进行平移得到新的点
VS_ALIGNBASE_API sc2Vector vfGetTransformShiftAlongRotateAnglePoint(
	sc2Vector vSrcPt, scDegree degAlongRotateAngle, double dShiftX, double dShiftY);



// 标定基准Mark与平台旋转D轴中心的相对关系（平台类型：相对于平台旋转D轴，相机独立安装）
// 根据基准Mark、旋转前后的图像坐标、旋转角度以及2X2矩阵，标定计算平台坐标系基准Mark相对于D轴旋转中心的偏移量
VS_ALIGNBASE_API BOOL vfCalibBenchMarkPlatformPos(
	sc2Vector vStartMarkImagePos, sc2Vector vRotatedMarkImagePos, scDegree degRotateAngle, 
	const sc2Matrix matrix, sc2Vector vBenchMarkImagePos, sc2Vector& vBenchMarkPlatformPos);

// 标定基准Mark与平台旋转D轴中心的相对关系（平台类型：相对于平台旋转D轴，相机独立安装）
// 根据基准Mark、旋转前后的图像坐标、旋转角度以及H矩阵，标定计算平台坐标系基准Mark相对于D轴旋转中心的偏移量
VS_ALIGNBASE_API BOOL vfCalibBenchMarkPlatformPos(
	sc2Vector vStartMarkImagePos, sc2Vector vRotatedMarkImagePos, scDegree degRotateAngle, 
	const double h[9], sc2Vector vBenchMarkImagePos, sc2Vector& vBenchMarkPlatformPos);	


// 为了标定求精基准Mark与平台旋转D轴中心的相对关系，先获取求精移动量（平台类型：标准XYD平台，相机独立安装）
// 根据原基准Mark的平台坐标、旋转角度,计算平台坐标系下基准Mark位置求精所需要XY移动量和D移动量（默认左手坐标系，未考虑轴的方向和类型）
VS_ALIGNBASE_API BOOL vfGetCalibRefineBenchMarkMoveOffset(sc2Vector vBenchMarkPlatformPos, scDegree degRotateAngle,
	double& dOffsetX, double& dOffsetY, scDegree& degOffsetD);

// 标定求精基准Mark与平台旋转D轴中心的相对关系（平台类型：标准XYD平台，相机独立安装）
// 根据原基准Mark旋转前后的平台坐标、旋转角度以及原基准Mark的平台坐标，求精计算平台坐标系基准Mark相对于旋转中心的偏移量
VS_ALIGNBASE_API BOOL vfCalibRefineBenchMarkPlatformPos(
	sc2Vector vRotatedPositiveMarkPlatformPos, sc2Vector vRotatedNegativeMarkPlatformPos, scDegree degRotateAngle, 
	sc2Vector vOldBenchMarkPlatformPos, sc2Vector& vNewBenchMarkPlatformPos);	



#pragma pack(pop)