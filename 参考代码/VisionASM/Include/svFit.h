#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svCircle.h"
#include "svEllipse.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

// 直线拟合
SV2DMEASURE_API bool sfFitLine(const svStd vector<sc2Vector>& points, scLine& line, double& rms, bool bConstrain=false, scRadian angleConstrain=scRadian(0));
SV2DMEASURE_API bool sfFitLineOutLine(const svStd vector<sc2Vector>& points, scLine& line, double& rms, int outcount, svStd vector<bool>& used, OutLineMethod method = eRobust, bool bConstrain=false, scRadian angleConstrain=scRadian(0));
SV2DMEASURE_API bool sfFitLineOutLine2(const svStd vector<sc2Vector>& points1, int outcount1, const svStd vector<sc2Vector>& points2, int outcount2, scLine& line, double& rms, svStd vector<bool>& used1, svStd vector<bool>& used2, bool bConstrain=false, scRadian angleConstrain=scRadian(0));

// 圆拟合
SV2DMEASURE_API bool sfFitCircle(const svStd vector<sc2Vector>& points, scCircle& circle, double& rms, FitMethod method, bool bConstrain=false, double radiusConstrain=10);
SV2DMEASURE_API bool sfFitCircleOutLine(const svStd vector<sc2Vector>& points, scCircle& circle, double& rms, int outcount, svStd vector<bool>& used, OutLineMethod method = eRobust);

// 椭圆拟合
SV2DMEASURE_API bool sfFitEllipse(const svStd vector<sc2Vector>& points, scEllipse& ellipse, double& rms, FitMethod method, bool bConstrain=false, scRadian angleConstrain=scRadian(0));
SV2DMEASURE_API bool sfFitEllipseOutLine(const svStd vector<sc2Vector>& points, scEllipse& ellipse, double& rms, int outcount, svStd vector<bool>& used, OutLineMethod method = eRobust);

// 同心圆拟合
SV2DMEASURE_API bool sfFitConcentricCircle(const svStd vector<sc2Vector>& points, const svStd vector<int>& nums, 
	sc2Vector& center, svStd vector<double>& radius, svStd vector<bool>& used, double& rms);

// 多线拟合
SV2DMEASURE_API bool sfFitMultiLine(const svStd vector<sc2Vector>& vPoints,double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxLines, svStd vector<scLine>& vLine, svStd vector<double>& vdRms, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);

// 多圆拟合
SV2DMEASURE_API bool sfFitMultiCircle(const svStd vector<sc2Vector>& vPoints,double dDistanceThre,int nMinPoints, MultiFitSortMethod eSortMode,int nMaxCircles, svStd vector<scCircle>& vCircle, svStd vector<double>& vdRms, svStd vector<bool>& vbUsed, svStd vector< svStd vector<int>>& vIndex);

#pragma pack(pop)