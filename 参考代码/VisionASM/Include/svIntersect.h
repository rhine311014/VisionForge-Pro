#pragma once

#include "svDef.h"
#include "svLine.h"
#include "svLineSeg.h"
#include "svCircle.h"
#include "svEllipse.h"
#include "svVector.h"

#pragma pack(push,8)

//圆与圆的交点
SV2DMEASURE_API bool sfInterSectCircleCircle(scCircle circle1, scCircle circle2, bool& bIntersect, svStd vector<sc2Vector>& points);

//直线与直线的交点
SV2DMEASURE_API bool sfInterSectLineLine(scLine line1, scLine line2, bool& bIntersect, svStd vector<sc2Vector>& points);

//直线与圆的交点
SV2DMEASURE_API bool sfInterSectLineCircle(scLine line, scCircle circle, bool& bIntersect, svStd vector<sc2Vector>& points);

//直线与椭圆的交点
SV2DMEASURE_API bool sfInterSectLineEllipse(scLine line, scEllipse ellipse, bool& bIntersect, svStd vector<sc2Vector>& points);

//线段与线段的交点
SV2DMEASURE_API bool sfInterSectSegmentSegment(scLineSeg segment1, scLineSeg segment2, bool& bIntersect, svStd vector<sc2Vector>& points);

//线段与直线的交点
SV2DMEASURE_API bool sfInterSectSegmentLine(scLineSeg segment, scLine line, bool& bIntersect, svStd vector<sc2Vector>& points);

//线段与圆的交点
SV2DMEASURE_API bool sfInterSectSegmentCircle(scLineSeg segment, scCircle circle, bool& bIntersect, svStd vector<sc2Vector>& points);

//线段与椭圆的交点
SV2DMEASURE_API bool sfInterSectSegmentEllipse(scLineSeg segment, scEllipse ellipse, bool& bIntersect, svStd vector<sc2Vector>& points);

#pragma pack(pop)