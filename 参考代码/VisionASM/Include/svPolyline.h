// svPolyline.h

#pragma once

#include "svShape.h"
#include <vector>

#pragma pack(push,8)



// ---------------------------------------------------------------------
//                            多边形
// ---------------------------------------------------------------------


class scPolyline;
smDerivedShapePtrhDecl(scPolyline);

class SVCORE_API scPolyline : public scShape  
{
public:

	explicit scPolyline(bool bClosed = false);									// 显式构造函数，若bClosed为true则构造闭合的多边形
	scPolyline(const svStd vector<sc2Vector> &vertices, bool bClosed = false);	// 用一组点构成的向量来构造多边形
	scPolyline(const scPolyline& polyline);

	int GetVerticesNum() const;													// 返回顶点的数量

	const sc2Vector& GetVertex(int idx) const;									// 返回第idx个顶点的坐标
	bool SetVertex(int idx, const sc2Vector& vertex);							// 设置第idx个顶点的坐标
	bool SetVertex(int idx, double vertexX, double vertexY);

	const svStd vector<sc2Vector>& GetVertices() const;							// 获取所有的顶点坐标
	void SetVertices(const svStd vector<sc2Vector>& vertices);					// 重新设置多边形的顶点

	bool InsertVertex(int idx, const sc2Vector& vertex);						// 在idx位置插入新的顶点
	bool InsertVertex(int idx, double vertexX, double vertexY);

	bool InsertVertices(int idx, const svStd vector<sc2Vector>& vertices);		// 在第idx位置插入一组顶点

	void AddVertex(const sc2Vector& vertex);									// 在多边形点序列的尾部添加新的顶点
	void AddVertex(double vertexX, double vertexY);

	void AddVertices(const svStd vector<sc2Vector>& vertices);					// 在多边形点序列的尾部添加一组顶点

	bool RemoveVertex(int idx);													// 删除第idx个顶点

	void RemoveVertices();														// 删除所有顶点


	bool IsClosed() const;														// 获取多边形是否闭合
	void IsClosed(bool closed);													// 设置多边形是否闭合

	bool operator==(const scPolyline &rhs) const;								// 重载==操作符
	bool operator!=(const scPolyline &rhs) const								// 重载!=操作符
	{ return !(*this == rhs);}			

	scPolyline& operator=(const scPolyline &rhs);								// 重载=操作符

	scPolyline Map(const sc2XformLinear& c) const;								// 对当前多边形进行二维几何变换（一致缩放及平移但没有切变和旋转）,得到新的多边形对象

	void Reserve(int n);														// 预分配存储n个顶点所需的内存

	int Capacity() const;														// 查询多边形能够存储点的数量,不会小于numVertices()的返回值



	virtual ShapeType GetType() const{ return ePolyLine; };							// 返回类型的枚举值
	scShapePtrh Clone() const;													// 复制当前多边形得到新的多边形
	bool IsOpenContour() const;													// 是否为开轮廓,如果IsClose()为真且多边形至少有一个顶点,则判定为开轮廓返回true
	bool IsRegion() const;														// 是否为区域,如果闭合且至少有一个点,则返回true
	bool IsFinite() const;														// 是否为有限的,总是返回true
	bool IsEmpty() const;														// 是否为空,如果多边形没有顶点,则返回true

	scRect BoundingBox() const;													// 获取多边形的最小外接矩形

	sc2Vector NearestPoint(const sc2Vector& p) const;							// 计算多边形上离点p最近的点

	bool Within(const sc2Vector& p) const;										// 判断点p是否在多边形内部

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;										// 判断给定图形rhs是否与当前图形相等，仅供内部使用


private:

	svStd vector<sc2Vector> m_vVertices;
	bool m_bClosed;

};

#pragma pack(pop)
