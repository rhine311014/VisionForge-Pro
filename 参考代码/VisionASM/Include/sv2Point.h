// sv2Point.h

#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            点（二维）
// ---------------------------------------------------------------------
// 矩形类sc2Point用来表示二维平面上的点,派生自图形基类scShape

class sc2Point;
smDerivedShapePtrhDecl(sc2Point);

class SVCORE_API sc2Point : public scShape
{
public:
	
	// 构造函数
	sc2Point():m_vect(0,0){};			
	
	// 析构函数						
	virtual ~sc2Point(){};
	
	// 用二维向量v构造
	sc2Point(const sc2Vector& v):m_vect(v){};		
	
	// 用x坐标和y坐标构造		
	sc2Point(double x, double y):m_vect(x,y){};				

	// 设置x坐标
	void SetX(double x){ m_vect.SetX(x);};
	
	// 设置y坐标
	void SetY(double y){ m_vect.SetY(y);};							

	// 返回x坐标
	double GetX() const{ return m_vect.GetX();};			
	
	// 返回y坐标			
	double GetY() const{ return m_vect.GetY();};						

	// 用二维向量vect参数设置点
	void SetVect(const sc2Vector& vect){ m_vect = vect;};		
	
	// 获取二维向量sc2Vect对象	
	const sc2Vector& GetVect() const { return m_vect;};			

	// 对当前点进行二维变换得到新的点
	sc2Point Map(const sc2XformLinear& c) { return sc2Point(c.MapPoint(m_vect)); };		

	bool operator==(const sc2Point& rhs) const;			// 重载==操作符
	bool operator!=(const sc2Point& rhs) const			// 重载!=操作符
	{return !(*this == rhs);}
	
	virtual ShapeType GetType() const{ return ePoint; };		// 返回类型的枚举值
	scShapePtrh Clone() const;							// 复制得到新的图形对象
	bool IsOpenContour() const;							// 是否为开的轮廓，总是返回TRUE
	bool IsRegion() const;								// 是否为区域，总是返回FALSE
	bool IsFinite() const;								// 是否为有限的，总是返回TRUE
	bool IsEmpty() const;								// 是否为空，总是返回FALSE

	// 获取最小外接矩形，总是返回一个尺寸为(0,0)的矩形
	scRect BoundingBox() const;			

	// 计算离二维向量p最近的点，总是返回当前点
	sc2Vector NearestPoint(const sc2Vector &p) const;	

	// 获取边长，总是返回0.0
	double Perimeter() const;
	
	// 对当前点进行二维变换得到新的点
//	scShapePtrh MapShape(const sc2XformLinear& X) const; 

private:
	bool IsEqual(const scShape& rhs) const;				// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	sc2Vector m_vect;			// 二维向量，用来保存当前点的坐标

};

#pragma pack(pop)
