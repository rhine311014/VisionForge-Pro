// svCircle.h

#pragma once

#include "svShape.h"
#include "svLine.h"

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            圆形
// ---------------------------------------------------------------------
// 

class scCircle;
smDerivedShapePtrhDecl(scCircle);


class SVCORE_API scCircle : public scShape  
{
public:
	scCircle(const sc2Vector& c=sc2Vector(0.0, 0.0), double r=1.0);			// 用圆心c和半径r来构造圆形

	scCircle(double cX ,double cY , double r);								// 用圆心c和半径r来构造圆形
		
	void SetCenterRadius(const sc2Vector& c, double r);						// 设置圆心c和半径r
	void SetCenterRadius(double cX ,double cY, double r);

	void SetCenter(double cX ,double cY);									// 设置圆心c
	void SetCenter(const sc2Vector& c);										
	void SetRadius(double r);												// 设置半径r

	const sc2Vector& GetCenter() const { return m_vCenter;};				// 获取圆心
	double GetRadius() const{ return m_dRadius;};							// 获取半径

	bool Set3Points(sc2Vector p1, sc2Vector p2, sc2Vector p3);

	// 根据弧度参数phi获取圆上的点
	sc2Vector GetPoint(const scRadian& phi) const
	{ return m_vCenter + sc2Vector(m_dRadius*cos(phi), m_dRadius*sin(phi));};

	scCircle Map(const sc2XformLinear& c) const;							// 对当前圆形进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的圆形

	// 判断圆的参数是否退化
	// 如果圆半径为非正值，则返回true，否则返回false
	bool Degenerate() const { return m_dRadius <= 0.; }						

	// 求给定线line与当前圆形的交点
	// 如果有交点则返回true，否则返回false
	// 返回true时，如果直线与圆相切，则point0 == point1；
	// 如果直线与圆相交，则point0和point2为两个交点
	bool Intersect(const scLine& line,										
		sc2Vector& point0, sc2Vector& point1) const;

//	bool Intersect(const scCircle &other,									// 求给定圆形other与当前圆形的交点
//		sc2Vector &point0, sc2Vector &point1) const;


	bool operator==(const scCircle& other) const;							// 重载==操作符
	bool operator!=(const scCircle& other) const							// 重载!=操作符
	{ return !(*this == other);}

	virtual ShapeType GetType() const{ return eCircle; };						// 返回类型的枚举值
	scShapePtrh Clone() const;												// 复制得到新的圆形
	bool IsOpenContour() const;												// 是否为开轮廓，总是返回false
	bool IsRegion() const;													// 是否为区域，总是返回true
	bool IsFinite() const;													// 是否为有限的，总是返回true
	bool IsEmpty() const;													// 是否为空的，总是返回false

	scRect BoundingBox() const;												// 获取当前圆形的最小外接矩形
	sc2Vector NearestPoint(const sc2Vector& p) const;						// 计算当前圆形上离给定点p最近的点
	double Perimeter() const;												// 获取当前圆形的周长

	bool Within(const sc2Vector& p) const;									// 判断给定点是否在当前圆形内

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;									// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	sc2Vector m_vCenter;
	double	  m_dRadius;

};

#pragma pack(pop)