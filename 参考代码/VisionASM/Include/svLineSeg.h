// svLineSeg.h


#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            线段
// ---------------------------------------------------------------------
// 
class scLine;
class scLineSeg;
smDerivedShapePtrhDecl(scLineSeg);

class SVCORE_API scLineSeg : public scShape  
{
public:
	scLineSeg();												// 构造函数
	scLineSeg (const sc2Vector& p1, const sc2Vector& p2);		// 用起始点p1和终止点p2构造线段对象
	
	void Set(const sc2Vector& p1, const sc2Vector& p2);			// 设置始点p1和终止点p2
	void Set(double p1X,double p1Y,double p2X,double p2Y);
	void SetP1(const sc2Vector& p);								// 设置起始点p1
	void SetP1(double pX,double pY);
	void SetP2(const sc2Vector& p);								// 设置终止点p2
	void SetP2(double pX,double pY);
	const sc2Vector& GetP1() const;								// 获取起始点p1
	const sc2Vector& GetP2() const;								// 获取终止点p2


	scLineSeg Map(const sc2XformLinear& c) const;				// 对当前线段对象进行sc2Xform变换c得到新的线段对象

	scLine GetLine() const;										// 返回当前线段所在的直线
	scRadian GetRotation() const;								// 获取旋转角度
	bool Degenerate() const { return m_p1 == m_p2;}				// 判断线段是否退化

	bool operator==(const scLineSeg& other) const;				// 重载==操作符
	bool operator!=(const scLineSeg& other) const				// 重载!=操作符
	{ return !(*this == other);}


	virtual ShapeType GetType() const{ return eLineSeg; };			// 返回类型的枚举值
	scShapePtrh Clone() const;									// 复制当前线段对象得到新的图形对象
	bool IsOpenContour() const;									// 判断是否为开轮廓,总是返回true
	bool IsRegion() const;										// 判断是否为区域,总是返回false
	bool IsFinite() const;										// 判断是否有限,总是返回true
	bool IsEmpty() const;										// 判断是否为空,总是返回false

	scRect BoundingBox() const;									// 获取线段的最小外接矩形
	sc2Vector NearestPoint(const sc2Vector &p) const;			// 计算当前线段上离给定点p最近的点
	double Perimeter() const;									// 获取线段长度,

private:
	bool IsEqual(const scShape& rhs) const;						// 判断给定图形rhs是否与当前图形相等，仅供内部使用


private:

	sc2Vector m_p1, m_p2;										//p1起始点,p2终止点

};

#pragma pack(pop)
