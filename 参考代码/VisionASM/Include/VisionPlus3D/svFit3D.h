#pragma once

#include "svVision3DDef.h"
#include "svVector.h"
#include "svSphere.h"
#include "svZeroPlaneResult.h"
#include "svLine3D.h"
#include "svEllipse3D.h"
#include "svPlane.h"

#pragma pack(push,8)

// 平面最小二乘拟合
// z = res[0] * x + res[1] * y + res[2]
SV_VISION_3D_API bool sfLeastSquaresPlaneFit(const svStd vector<sc3Vector>& vPoints,svStd vector<double>& vdWeight,sc3Vector& res);

// 球面最小二乘拟合
SV_VISION_3D_API bool sfLeastSquaresSphereFit(const svStd vector<sc3Vector>& vPoints,svStd vector<double>& vdWeight,sc3Vector& center,double& radius);

// 直线最小二乘拟合
SV_VISION_3D_API bool sfLeastSquareLine3DFit(const std::vector<sc3Vector> vFitPoint, scLine3D& lineRes);

SV_VISION_3D_API bool sfWeightLeastSquareLine3DFit(const std::vector<sc3Vector> vFitPoint, std::vector<double>& vdWeight, scLine3D& lineRes);

//平面拟合
SV_VISION_3D_API bool sfRansacThreHuberAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacThreTukeyAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineHuberThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineHuberAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfRansacOutLine3DPlaneFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfHuberAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, const scZeroPlaneResult& RansacBestPlane, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, const scZeroPlaneResult& RansacBestPlane, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfHuberAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfHuberThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfHuberThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, const scZeroPlaneResult& RansacBestPlane, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, const scZeroPlaneResult& RansacBestPlane, double dDistanceThreshold, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyOutLine3DPlaneFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyOutLine3DPlaneFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, const scZeroPlaneResult& RansacBestPlane, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyOutLine3DPlaneFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99, int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfRansacAutoThreTukeyAutoThre3DPlaneFit(const svStd vector<sc3Vector>& vPoints, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThreTukeyOutLine3DPlaneFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scZeroPlaneResult& BestPlane, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99, int nIter = 100,double dAccuracy = 0.001);

// 多平面拟合
SV_VISION_3D_API bool sfFitPlanes(const std::vector<sc3Vector>& vPoints, double  dThre, int  nMinPoints, int nMaxPlanes, int nMode, std::vector<scZeroPlaneResult>& vPlanes, std::vector<double>& vdRms, std::vector<bool>& vbUsed, std::vector<std::vector<int>>& viIndex);

// 球面拟合
SV_VISION_3D_API bool sfRansacAutoThreSphereFit(const svStd vector<sc3Vector>& vPoints, scSphere& SphereResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfRansacOutLineSphereFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scSphere& SphereResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfTukeyAutoThreSphereFit(const svStd vector<sc3Vector>& vPoints, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyAutoThreSphereFit(const svStd vector<sc3Vector>& vPoints, const scSphere& RansacSphereResult, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyOutLineSphereFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyOutLineSphereFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, const scSphere& RansacSphereResult, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyAutoThreSphereFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyOutLineSphereFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThreTukeyOutLineSphereFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThreTukeyAutoThreSphereFit(const svStd vector<sc3Vector>& vPoints, scSphere& SphereResult, double& dRms, svStd vector<double>& vdWeight, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

// 直线拟合
SV_VISION_3D_API bool sfRansacOutLine3DLineFit(const std::vector<sc3Vector>& vPoints, int nOutPercent, double dProbSolution, scLine3D& lineRes, double& dRms, std::vector<bool>& vbUsed);

SV_VISION_3D_API bool sfRansacAutoThre3DLineFit(const std::vector<sc3Vector>& vPoints, double dProbSolution, scLine3D& lineRes, double& dRms, std::vector<bool>& vbUsed);

SV_VISION_3D_API bool sfTukeyOutLine3DLineFit(const std::vector<sc3Vector>& vPoints, int nOutPer, int nIter, double dAccuracy, scLine3D& lineRes, double& dRms, std::vector<bool>& vbUsed, std::vector<double>& vdWeight);

SV_VISION_3D_API bool sfTukeyAutoThre3DLineFit(const std::vector<sc3Vector>& vPoints, int nIter, double dAccuracy, scLine3D& lineRes, double& dRms, std::vector<bool>& vbUsed, std::vector<double>& vdWeight);

SV_VISION_3D_API bool sfRansacTukey3DLineFit(const std::vector<sc3Vector>& vPoints, int nIter, double dAccuracy, int nOutPercent, double dProbSolution, bool bRansacAutoThresh, bool bTukeyAutoThresh, scLine3D& lineRes, double& dRms, std::vector<bool>& vbUsed);

// 椭圆拟合
SV_VISION_3D_API bool sfRansacAutoThreEllipse3DFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfRansacOutLineEllipse3DFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99);

SV_VISION_3D_API bool sfTukeyAutoThreEllipse3DFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfTukeyOutLineEllipse3DFit(const svStd vector<sc3Vector>& vPoints,int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyOutLineEllipse3DFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacOutLineTukeyAutoThreEllipse3DFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThreTukeyAutoThreEllipse3DFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

SV_VISION_3D_API bool sfRansacAutoThreTukeyOutLineEllipse3DFit(const svStd vector<sc3Vector>& vPoints, int nOutPer, scEllipse3D& Ellipse3DResult, double& dRms, svStd vector<bool>& vbUsed, double dProbSolution = 0.99,int nIter = 100,double dAccuracy = 0.001);

#pragma pack(pop)
