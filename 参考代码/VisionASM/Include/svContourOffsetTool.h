#pragma once

#include "svPolylineArc.h"

#pragma pack(push,8)

class scCircularArc;

// 轮廓内缩外扩工具
class SVCORE_API scContourOffsetTool
{
public:
	scContourOffsetTool();
	~scContourOffsetTool();

	enum ContourOffsetType
	{
		eRetraction = 0,  // 内缩
		eOutreach,        // 外扩
	};

	// 设置偏移方向
	void SetOffsetType(ContourOffsetType type);
	ContourOffsetType GetOffsetType()const;

	// 设置偏移位置
	void SetOffsetPos(sc2Vector pos);
	sc2Vector GetOffsetPos()const;

	// 设置偏移距离
	void SetOffsetDistance(double dDis);
	double GetOffsetDistance()const;

	// 设置是否处理自交
	void SetProcSelfintersect(bool bProcSelfintersect);
	bool GetProcSelfintersect()const;

	// 执行轮廓检测
	// bOffsetByPos : true - 按位置进行偏移; false - 按偏移方向进行偏移
	// bInvertVertexPorc : true - 将轮廓顶点反向后再进行处理，false - 处理原轮廓
	bool Execute(const scPolylineArc& polylineArc,bool bOffsetByPos = false,bool bInvertVertexPorc = false);

	// 获取结果轮廓数量
	int GetContourResultNum()const;

	// 获取结果轮廓
	scPolylineArc GetContourResult(int nIndex = 0)const;

	// 获取所有结果轮廓
	std::vector<scPolylineArc> GetContourResultAll()const;

	// 起始点、终止点、凸度获取圆弧
	static scCircularArc GetArcFromEndpointAndConvex(sc2Vector startPoint,sc2Vector endPoint,double dConvex);

private:
	// 轮廓是否可用
	bool IsValidContour(const scPolylineArc& contour)const;

	// 轮廓是否为顺时针
	bool IsClockWise(const scPolylineArc& contour)const;

	// 计算轮廓顶点平坦度
	std::vector<int> ComputeConcavity(const scPolylineArc& contour,bool bClockWise)const;

	// 检查所有点是否均为平坦点
	bool IsAllFlat(const std::vector<int>& vConcavity)const;

	// 计算偏移方向
	std::vector<sc2Vector> ComputeVerticesOffsetDir(const scPolylineArc& contour,ContourOffsetType offsetType,std::vector<int>& vConcavity)const;

	// 偏移方向扩散
	void ExpandDirToFlatPoint(std::vector<int>& vConcavity,std::vector<sc2Vector>& vVerticesOffsetDir,bool bClosed)const;

private:
	ContourOffsetType m_ContourOffsetType;
	sc2Vector m_vOffsetPos;
	double m_dOffsetDistance;
	bool m_bProcSelfintersect;

	std::vector<scPolylineArc> m_vResultContour;
};

#pragma pack(pop)