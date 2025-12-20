// svCrossPoint.h

#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            点（二维）
// ---------------------------------------------------------------------
// 矩形类scCrossPoint用来表示二维平面上的点,派生自图形基类scShape

class scCrossPoint;
smDerivedShapePtrhDecl(scCrossPoint);

class SVCORE_API scCrossPoint : public scShape
{
public:
	
	// 构造函数
	scCrossPoint();			
	
	// 析构函数						
	virtual ~scCrossPoint();
	
	// 用二维向量v构造
	scCrossPoint(const sc2Vector& center, const sc2Vector& length, const scRadian& rRotation = scRadian(0));		
	
	// 用x坐标和y坐标构造		
	scCrossPoint(double cx, double cy, double lx, double ly, double dRotation = 0);				

	// 设置x坐标
	void SetCenterX(double x){ m_vCenter.SetX(x);};
	
	// 设置y坐标
	void SetCenterY(double y){ m_vCenter.SetY(y);};							

	// 返回x坐标
	double GetCenterX() const{ return m_vCenter.GetX();};			
	
	// 返回y坐标			
	double GetCenterY() const{ return m_vCenter.GetY();};		

	// 设置x长度
	void SetLengthX(double x){ m_vLength.SetX(x);};

	// 设置y长度
	void SetLengthY(double y){ m_vLength.SetY(y);};							

	// 返回x长度
	double GetLengthX() const{ return m_vLength.GetX();};			

	// 返回y长度			
	double GetLengthY() const{ return m_vLength.GetY();};

	// 设置Rotation角度
	void SetRotation(scRadian rRotation){ m_rRotation = rRotation;};							

	// 返回Rotation角度
	scRadian GetRotation() const{ return m_rRotation;};			

	// 用二维向量vect参数设置点
	void SetCenter(const sc2Vector& vect){ m_vCenter = vect;};		
	
	// 获取二维向量sc2Vect对象	
	const sc2Vector& GetCenter() const { return m_vCenter;};	

	// 用二维向量vect参数设置长度
	void SetLength(const sc2Vector& vect){ m_vLength = vect;};		

	// 获取二维向量sc2Vect对象	
	const sc2Vector& GetLength() const { return m_vLength;};

	// 对当前点进行二维变换得到新的点
	scCrossPoint Map(const sc2XformLinear& c) { return scCrossPoint(c.MapPoint(m_vCenter),m_vLength,m_rRotation); };		

	bool operator==(const scCrossPoint& rhs) const;			// 重载==操作符
	bool operator!=(const scCrossPoint& rhs) const			// 重载!=操作符
	{return !(*this == rhs);}
	
	virtual ShapeType GetType() const{ return eCrossPoint; };		// 返回类型的枚举值
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
	sc2Vector m_vCenter;			// 二维向量，用来保存当前交叉点的坐标
	sc2Vector m_vLength;			// 二维向量，用来保存当前交叉点的大小
	scRadian  m_rRotation;			// 交叉线的旋转角度
};

#pragma pack(pop)
