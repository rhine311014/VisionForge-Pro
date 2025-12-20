// svPolylineArc.h

#pragma once

#include "svShape.h"
#include <vector>
#include "svPolyline.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            多段线
// ---------------------------------------------------------------------

class SVCORE_API scVertexData
{
public:
	scVertexData();
	scVertexData(sc2Vector vertex, double dConv, bool bLineSeg);
	virtual ~scVertexData();

	void SetVertex(sc2Vector vertex);
	sc2Vector GetVertex() const;

	void SetConvex(double dConv);
	double GetConvex() const;

	void EnableLineSeg(bool bEnable);
	bool IsLineSeg() const;

	scVertexData Map(const sc2XformLinear& c) const;

private:
	sc2Vector	m_vVertex;		//坐标
	double		m_dConvex;		//凸度
	bool		m_bLineSeg;	
};

class scPolylineArc;
smDerivedShapePtrhDecl(scPolylineArc);

class SVCORE_API scPolylineArc : public scShape  
{
public:

	explicit scPolylineArc(bool bClosed = false);								// 显式构造函数，若bClosed为true则构造闭合的多段线
	scPolylineArc(const std::vector<scVertexData> &vVertex, bool bClosed = false);	// 用一组点构成的向量来构造多边形
	scPolylineArc(const scPolylineArc& polylineArc);
	scPolylineArc(const scPolyline& polyline);

	bool SetVertex(int nIdx, const scVertexData& vertex);						// 设置第nIdx个顶点
	bool GetVertex(int nIdx, scVertexData& vertex) const;							// 返回第nIdx个顶点

	void SetVertices(const std::vector<scVertexData>& vertices);					// 重新设置多段线的顶点
	const std::vector<scVertexData>& GetVertices() const;							// 获取所有的顶点数据

	bool InsertVertex(int nIdx, const scVertexData& vertex);						// 在nIdx位置插入新的顶点

	bool InsertVertices(int nIdx, const std::vector<scVertexData>& vertices);		// 在第nIdx位置插入一组顶点

	void AddVertex(const scVertexData& vertex);									// 在多段线点序列的尾部添加新的顶点

	void AddVertices(const std::vector<scVertexData>& vertices);					// 在多段线点序列的尾部添加一组顶点

	bool RemoveVertex(int nIdx);												// 删除第nIdx个顶点

	void RemoveVertices();														// 删除所有顶点

	void SetClosed(bool bClosed);												// 设置多段线是否闭合
	bool IsClosed() const;														// 获取多段线是否闭合

	int GetVerticesNum() const;													// 返回顶点的数量

	scPolylineArc& operator=(const scPolylineArc &rhs);							// 重载=操作符
	bool operator==(const scPolylineArc &rhs) const;							// 重载==操作符
	bool operator!=(const scPolylineArc &rhs) const								// 重载!=操作符
	{ return !(*this == rhs);}			

	scPolylineArc Map(const sc2XformLinear& c) const;							// 对当前多边形进行二维几何变换（一致缩放及平移但没有切变和旋转）,得到新的多边形对象

	void Reserve(int n);														// 预分配存储n个顶点所需的内存

	virtual ShapeType GetType() const{ return ePolylineArc; };					// 返回类型的枚举值
	scShapePtrh Clone() const;													// 复制当前多边形得到新的多段线
	bool IsOpenContour() const;													// 是否为开轮廓
	bool IsRegion() const;														// 是否为区域
	bool IsFinite() const;														// 是否为有限的,总是返回true
	bool IsEmpty() const;														// 是否为空,如果多边形没有顶点,则返回true

	scRect BoundingBox() const;													// 获取多段线的最小外接矩形

	sc2Vector NearestPoint(const sc2Vector& p) const;							// 计算多段线上离点p最近的点

	bool Within(const sc2Vector& p) const;										// 判断点p是否在多段线内部

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;										// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	std::vector<scVertexData> m_vVertexData;
	bool m_bClosed;
};

#pragma pack(pop)
