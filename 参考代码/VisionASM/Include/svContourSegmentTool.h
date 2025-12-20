#pragma once

#include "svPolylineArc.h"
#include "svFeature.h"
#include "svContourFilterTool.h"
#include "svCircularArc.h"

#pragma pack(push,8)

// 轮廓分割
class SVCORE_API scContourSegmentTool
{
public:
	scContourSegmentTool();
	~scContourSegmentTool();

	// 先通过轮廓过滤获得顶点索引，再进行圆弧拟合，分割轮廓
	bool Execute(const scPolyline& inputContour,int nSmoothPara,double dDisThre);
	bool Execute(const scFeatureletVector& inputContour,bool bClosed,int nSmoothPara,double dDisThre);
	bool Execute(const std::vector<sc2Vector>& inputPoints,bool bClosed,int nSmoothPara,double dDisThre);
	bool Execute(const scFeatureletChainSet& inputContour,int nSmoothPara,double dDisThre);

	// 通过顶点索引进行圆弧拟合，将轮廓分割
	bool Execute(const scPolyline& inputContour,const std::vector<int>& vContourIndex);
	bool Execute(const scFeatureletVector& inputContour,bool bClosed,const std::vector<int>& vContourIndex);
	bool Execute(const std::vector<sc2Vector>& inputPoints,bool bClosed,const std::vector<int>& vContourIndex);
	bool Execute(const scFeatureletChainSet& inputContour,const std::vector<std::vector<int>>& vContourIndex);

	// 获取分割结果
	scPolylineArc GetSegmentResult(int nIndex = 0)const;

	// 获取所有分割结果
	std::vector<scPolylineArc> GetSegmentResultAll()const;

	// 获取过滤工具，可获取过滤过程的结果
	scContourFilterTool* GetContourFilterTool();

private:
	// 检查输入轮廓是否可用
	bool IsContourValid(const scPolyline& contour,const std::vector<int>& vContourIndex)const;

	// 已知两端点的圆弧拟合
	bool CircleArcFitConstraint(const std::vector<sc2Vector>& vPoints,sc2Vector p1,sc2Vector p2,scCircularArc& cirArc);
	bool CircleArcFitConstraint(const std::vector<sc2Vector>& vPoints,int nFirst,int nEnd,sc2Vector p1,sc2Vector p2,scCircularArc& cirArc);

	// 3点转换为圆弧
	bool ThreePointsTransToArc(sc2Vector p1,sc2Vector p2,sc2Vector p3,scCircularArc& cirArc);

private:
	scContourFilterTool m_ContourFilterTool;
	std::vector<scPolylineArc> m_vContourResult;
};

#pragma pack(pop)