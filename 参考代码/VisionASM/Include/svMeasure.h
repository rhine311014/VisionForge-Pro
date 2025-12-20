#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svCircle.h"
#include "svEllipse.h"
#include "svVector.h"

#pragma pack(push,8)

//点到点的角度测量
SV2DMEASURE_API bool sfAnglePoint2Point(sc2Vector point1, sc2Vector point2, scRadian& angle);

//直线到直线的角度测量
SV2DMEASURE_API bool sfAngleLine2Line(scLine line1, scLine line2, scRadian& angle);

//点到点的距离测量
SV2DMEASURE_API bool sfDistancePoint2Point(sc2Vector point1, sc2Vector point2, double& distance);

//点到线段的距离测量
SV2DMEASURE_API bool sfDistancePoint2Segment(sc2Vector point, scLineSeg segment, double& distance, sc2Vector& pointSegment);

//点到直线的距离测量
SV2DMEASURE_API bool sfDistancePoint2Line(sc2Vector point, scLine line, double& distance, sc2Vector& pointLine);

//点到圆的距离测量
SV2DMEASURE_API bool sfDistancePoint2Circle(sc2Vector point, scCircle circle, double& distance, sc2Vector& pointCircle);

//点到椭圆的距离测量
SV2DMEASURE_API bool sfDistancePoint2Ellipse(sc2Vector point, scEllipse ellipse, double& distance, sc2Vector& pointEllipse);

//直线到圆的距离测量
SV2DMEASURE_API bool sfDistanceLine2Circle(scLine line, scCircle circle, double& distance, sc2Vector& pointLine, sc2Vector& pointCircle);

//直线到椭圆的距离测量
SV2DMEASURE_API bool sfDistanceLine2Ellipse(scLine line, scEllipse ellipse, double& distance, sc2Vector& pointLine, sc2Vector& pointEllipse);

//圆到圆的距离测量
SV2DMEASURE_API bool sfDistanceCircle2Circle(scCircle circle1, scCircle circle2, double& distance, sc2Vector& pointCircle1, sc2Vector& pointCircle2);

//线段到线段的距离测量
SV2DMEASURE_API bool sfDistanceSegment2Segment(scLineSeg segment1, scLineSeg segment2, double& distance, sc2Vector& pointSegment1, sc2Vector& pointSegment2);

//线段到直线的距离测量
SV2DMEASURE_API bool sfDistanceSegment2Line(scLineSeg segment, scLine line, double& distance, sc2Vector& pointSegment, sc2Vector& pointLine);

//线段到圆的距离测量
SV2DMEASURE_API bool sfDistanceSegment2Circle(scLineSeg segment, scCircle circle, double& distance, sc2Vector& pointSegment, sc2Vector& pointCircle);

//线段到椭圆的距离测量
SV2DMEASURE_API bool sfDistanceSegment2Ellipse(scLineSeg segment, scEllipse ellipse, double& distance, sc2Vector& pointSegment, sc2Vector& pointEllipse);

#pragma pack(pop)